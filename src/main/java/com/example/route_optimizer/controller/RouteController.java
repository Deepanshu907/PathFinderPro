package com.example.route_optimizer.controller;

import com.example.route_optimizer.service.GraphService;
import com.example.route_optimizer.service.PathResponse;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "*") // allow frontend calls
public class RouteController {

    private final GraphService graphService;

    public RouteController(GraphService graphService) {
        this.graphService = graphService;
    }

    @GetMapping("/dijkstra")
    public PathResponse getShortestPath(@RequestParam char src, @RequestParam char dest) {
        return graphService.dijkstra(src, dest);
    }

    @GetMapping("/bellman-ford")
    public PathResponse getBellmanFord(@RequestParam char src, @RequestParam char dest) {
        return graphService.bellmanFord(src, dest);
    }

    @GetMapping("/astar")
    public PathResponse getAStar(@RequestParam char src, @RequestParam char dest) {
        return graphService.aStar(src, dest);
    }

    // Handle invalid inputs gracefully
    @ExceptionHandler(IllegalArgumentException.class)
    public ResponseEntity<Map<String, String>> handleIllegalArgument(IllegalArgumentException ex) {
        Map<String, String> error = new HashMap<>();
        error.put("error", ex.getMessage());
        return new ResponseEntity<>(error, HttpStatus.BAD_REQUEST);
    }
}
