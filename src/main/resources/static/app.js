// app.js - draw map, handle clicks, call backend

// city coordinates (copied from your C++ layout)
const cityPos = {
  A: {x:30,  y:50},   B:{x:230, y:50},
  C:{x:430, y:50},    D:{x:610, y:50},
  E:{x:110, y:180},   F:{x:310, y:180},
  G:{x:510, y:180},   H:{x:30,  y:320},
  I:{x:230, y:320},   J:{x:430, y:320},
  K:{x:610, y:320},   L:{x:40,  y:460},
  M:{x:190, y:460},   N:{x:330, y:460},
  O:{x:470, y:460},   P:{x:620, y:460}
};

// adjacency matrix (to draw edges visually)
const adjMatrix = [
  [0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0],
  [5,0,3,0,5,3,0,0,0,0,0,0,0,0,17,0],
  [0,3,0,6,0,8,0,0,0,0,0,0,0,0,0,0],
  [0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0],
  [6,5,0,0,0,3,0,4,3,5,0,0,0,0,0,0],
  [0,3,8,0,3,0,5,0,0,3,0,0,0,0,0,0],
  [0,0,0,6,0,5,0,0,0,0,4,0,0,0,0,0],
  [0,0,0,0,4,0,0,0,0,0,0,3,0,0,0,0],
  [0,0,0,0,3,0,0,0,0,4,0,3,5,0,15,0],
  [0,0,0,0,5,3,0,0,4,0,4,0,5,4,6,0],
  [0,0,0,0,0,0,4,0,0,4,0,0,0,3,4,5],
  [0,0,0,0,0,0,0,3,3,0,0,0,2,0,0,0],
  [0,0,0,0,0,0,0,0,5,5,0,2,0,2,0,0],
  [0,0,0,0,0,0,0,0,0,4,3,0,2,0,0,0],
  [0,17,0,0,0,0,0,0,0,6,4,0,0,0,0,2],
  [0,0,0,0,0,0,0,0,0,0,5,0,0,0,2,0]
];

const canvas = document.getElementById('cityCanvas');
const ctx = canvas.getContext('2d');
const radius = 18;
let selected = { src: null, dest: null };
let lastPath = null;

function letter(i){ return String.fromCharCode(65 + i); }
function toIndex(ch){ return ch.charCodeAt(0) - 65; }

function drawMap() {
  ctx.clearRect(0,0,canvas.width,canvas.height);

  // draw edges
  for(let i=0;i<adjMatrix.length;i++){
    for(let j=i+1;j<adjMatrix[i].length;j++){
      const w = adjMatrix[i][j];
      if(w>0){
        const a = cityPos[letter(i)];
        const b = cityPos[letter(j)];
        ctx.beginPath();
        ctx.moveTo(a.x, a.y);
        ctx.lineTo(b.x, b.y);
        ctx.lineWidth = 2;
        ctx.strokeStyle = '#c7ccd1';
        ctx.stroke();
      }
    }
  }

  // draw nodes
  Object.keys(cityPos).forEach(ch => {
    const p = cityPos[ch];
    ctx.beginPath();
    ctx.arc(p.x, p.y, radius, 0, Math.PI*2);
    ctx.fillStyle = (selected.src === ch || selected.dest === ch) ? '#fde68a' : '#ffffff';
    ctx.fill();
    ctx.lineWidth = 2;
    ctx.strokeStyle = '#111827';
    ctx.stroke();
    ctx.fillStyle = '#111827';
    ctx.font = 'bold 14px Arial';
    ctx.fillText(ch, p.x-6, p.y+6);
  });

  if(lastPath) highlightPath(lastPath);
}

canvas.addEventListener('click', (e)=>{
  const rect = canvas.getBoundingClientRect();
  const x = e.clientX - rect.left;
  const y = e.clientY - rect.top;
  for(const [ch,p] of Object.entries(cityPos)){
    if(Math.hypot(x-p.x, y-p.y) < radius+5){
      if(!selected.src){
        selected.src = ch;
        document.getElementById('srcInput').value = ch;
      } else if(!selected.dest && ch !== selected.src){
        selected.dest = ch;
        document.getElementById('destInput').value = ch;
        doSearch(selected.src, selected.dest);
      } else {
        selected = {src: ch, dest: null};
        document.getElementById('srcInput').value = ch;
        document.getElementById('destInput').value = '';
      }
      drawMap();
      break;
    }
  }
});

document.getElementById('goBtn').addEventListener('click', ()=>{
  const s = document.getElementById('srcInput').value.toUpperCase();
  const d = document.getElementById('destInput').value.toUpperCase();
  if(s && d && s !== d){
    selected.src = s; selected.dest = d;
    doSearch(s,d);
  }
});

document.getElementById('resetBtn').addEventListener('click', ()=>{
  selected = {src:null, dest:null};
  lastPath = null;
  document.getElementById('srcInput').value = '';
  document.getElementById('destInput').value = '';
  document.getElementById('result').textContent = 'Select source & destination or click nodes on the map.';
  drawMap();
});

async function doSearch(src, dest){
  const resultDiv = document.getElementById('result');
  resultDiv.textContent = 'Contacting server...';
  try {
    const res = await fetch(`/api/dijkstra?src=${src}&dest=${dest}`);
    const data = await res.json();
    resultDiv.innerHTML = `<strong>Shortest distance:</strong> ${data.distance} &nbsp; <strong>Path:</strong> ${data.path.join(' → ')}`;
    lastPath = data.path;
    drawMap();
  } catch(err){
    resultDiv.textContent = 'Error: ' + err.message;
  }
}

function highlightPath(path){
  if(path.length<2) return;
  ctx.lineWidth = 6;
  ctx.strokeStyle = '#10b981';
  for(let i=0;i<path.length-1;i++){
    const a = cityPos[path[i]], b = cityPos[path[i+1]];
    ctx.beginPath();
    ctx.moveTo(a.x,a.y); ctx.lineTo(b.x,b.y);
    ctx.stroke();
  }
}

drawMap();
