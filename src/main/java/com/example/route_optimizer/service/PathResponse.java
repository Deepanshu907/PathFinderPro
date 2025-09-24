package com.example.route_optimizer.service;

import java.util.List;

public class PathResponse {
    private int distance;
    private List<Character> path;

    public PathResponse(int distance, List<Character> path) {
        this.distance = distance;
        this.path = path;
    }

    public int getDistance() {
        return distance;
    }

    public List<Character> getPath() {
        return path;
    }
}
