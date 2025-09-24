package com.example.route_optimizer.service;

import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class GraphService {

    private static final int NUM_CITIES = 16;
    private static final int INF = Integer.MAX_VALUE;

    // Adjacency matrix (positive weights)
    private final int[][] adjMatrixDijkstra = {
            {0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0},
            {5,0,3,0,5,3,0,0,0,0,0,0,0,0,0,0},
            {0,3,0,6,0,8,0,0,0,0,0,0,0,0,0,0},
            {0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0},
            {6,5,0,0,0,3,0,4,3,5,0,0,0,0,0,0},
            {0,3,8,0,3,0,5,0,0,3,0,0,0,0,0,0},
            {0,0,0,6,0,5,0,0,0,0,4,0,0,0,0,0},
            {0,0,0,0,4,0,0,0,0,0,0,3,0,0,0,0},
            {0,0,0,0,3,0,0,0,0,4,0,3,5,0,0,0},
            {0,0,0,0,5,3,0,0,4,0,4,0,5,4,6,0},
            {0,0,0,0,0,0,4,0,0,4,0,0,0,3,4,5},
            {0,0,0,0,0,0,0,3,3,0,0,0,2,0,0,0},
            {0,0,0,0,0,0,0,0,5,5,0,2,0,2,0,0},
            {0,0,0,0,0,0,0,0,0,4,3,0,2,0,0,0},
            {0,0,0,0,0,0,0,0,0,6,4,0,0,0,0,2},
            {0,0,0,0,0,0,0,0,0,0,5,0,0,0,2,0}
    };

    // Coordinates of cities (for A* heuristic)
    private final int[][] cityCoords = {
            {30, 50},   {230, 50},  {430, 50},  {610, 50},
            {110, 180}, {310, 180}, {510, 180}, {30, 320},
            {230, 320}, {430, 320}, {610, 320}, {40, 460},
            {190, 460}, {330, 460}, {470, 460}, {620, 460}
    };

    private int getCityIndex(char city) {
        city = Character.toUpperCase(city);
        if (city >= 'A' && city <= 'P') {
            return city - 'A';
        }
        return -1;
    }

    private double heuristic(int a, int b) {
        int dx = cityCoords[a][0] - cityCoords[b][0];
        int dy = cityCoords[a][1] - cityCoords[b][1];
        return Math.sqrt(dx * dx + dy * dy);
    }

    // ---------- Dijkstra ----------
    public PathResponse dijkstra(char sourceChar, char destChar) {
        int src = getCityIndex(sourceChar);
        int dest = getCityIndex(destChar);

        if (src == -1 || dest == -1) {
            throw new IllegalArgumentException("Invalid city (must be A-P)");
        }

        int[] dist = new int[NUM_CITIES];
        int[] parent = new int[NUM_CITIES];
        Arrays.fill(dist, INF);
        Arrays.fill(parent, -1);

        dist[src] = 0;
        TreeSet<int[]> activeCities = new TreeSet<>(Comparator.comparingInt(a -> a[0]));
        activeCities.add(new int[]{0, src});

        while (!activeCities.isEmpty()) {
            int[] top = activeCities.pollFirst();
            int u = top[1];

            for (int v = 0; v < NUM_CITIES; v++) {
                if (adjMatrixDijkstra[u][v] > 0 && dist[u] + adjMatrixDijkstra[u][v] < dist[v]) {
                    activeCities.remove(new int[]{dist[v], v});
                    dist[v] = dist[u] + adjMatrixDijkstra[u][v];
                    parent[v] = u;
                    activeCities.add(new int[]{dist[v], v});
                }
            }
        }

        List<Character> path = new ArrayList<>();
        for (int v = dest; v != -1; v = parent[v]) {
            path.add((char) ('A' + v));
        }
        Collections.reverse(path);

        return new PathResponse(dist[dest], path);
    }

    // ---------- Bellman-Ford ----------
    public PathResponse bellmanFord(char sourceChar, char destChar) {
        int src = getCityIndex(sourceChar);
        int dest = getCityIndex(destChar);
        if (src == -1 || dest == -1) {
            throw new IllegalArgumentException("Invalid city (must be A-P)");
        }

        int[] dist = new int[NUM_CITIES];
        int[] parent = new int[NUM_CITIES];
        Arrays.fill(dist, INF);
        Arrays.fill(parent, -1);
        dist[src] = 0;

        // Relax edges NUM_CITIES - 1 times
        for (int i = 0; i < NUM_CITIES - 1; i++) {
            for (int u = 0; u < NUM_CITIES; u++) {
                for (int v = 0; v < NUM_CITIES; v++) {
                    if (adjMatrixDijkstra[u][v] > 0 && dist[u] != INF &&
                            dist[u] + adjMatrixDijkstra[u][v] < dist[v]) {
                        dist[v] = dist[u] + adjMatrixDijkstra[u][v];
                        parent[v] = u;
                    }
                }
            }
        }

        // Optional: detect negative cycle
        for (int u = 0; u < NUM_CITIES; u++) {
            for (int v = 0; v < NUM_CITIES; v++) {
                if (adjMatrixDijkstra[u][v] > 0 && dist[u] != INF &&
                        dist[u] + adjMatrixDijkstra[u][v] < dist[v]) {
                    throw new RuntimeException("Graph contains a negative weight cycle!");
                }
            }
        }

        List<Character> path = new ArrayList<>();
        for (int v = dest; v != -1; v = parent[v]) {
            path.add((char) ('A' + v));
        }
        Collections.reverse(path);

        return new PathResponse(dist[dest], path);
    }

    // ---------- A* ----------
    public PathResponse aStar(char sourceChar, char destChar) {
        int src = getCityIndex(sourceChar);
        int dest = getCityIndex(destChar);
        if (src == -1 || dest == -1) {
            throw new IllegalArgumentException("Invalid city (must be A-P)");
        }

        int[] parent = new int[NUM_CITIES];
        Arrays.fill(parent, -1);

        double[] gScore = new double[NUM_CITIES]; // cost from start
        Arrays.fill(gScore, Double.POSITIVE_INFINITY);
        gScore[src] = 0;

        double[] fScore = new double[NUM_CITIES]; // estimated total cost
        Arrays.fill(fScore, Double.POSITIVE_INFINITY);
        fScore[src] = heuristic(src, dest);

        PriorityQueue<Integer> openSet =
                new PriorityQueue<>(Comparator.comparingDouble(node -> fScore[node]));
        openSet.add(src);

        while (!openSet.isEmpty()) {
            int u = openSet.poll();

            if (u == dest) {
                List<Character> path = new ArrayList<>();
                for (int v = dest; v != -1; v = parent[v]) {
                    path.add((char) ('A' + v));
                }
                Collections.reverse(path);
                return new PathResponse((int) gScore[dest], path);
            }

            for (int v = 0; v < NUM_CITIES; v++) {
                if (adjMatrixDijkstra[u][v] > 0) {
                    double tentativeG = gScore[u] + adjMatrixDijkstra[u][v];
                    if (tentativeG < gScore[v]) {
                        parent[v] = u;
                        gScore[v] = tentativeG;
                        fScore[v] = gScore[v] + heuristic(v, dest);
                        openSet.add(v);
                    }
                }
            }
        }

        return new PathResponse(Integer.MAX_VALUE, List.of()); // No path
    }
}
