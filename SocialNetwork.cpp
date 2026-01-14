// SocialNetwork.cpp (single-file version for easy copy-paste)
// Build: g++ -std=c++17 SocialNetwork.cpp -o social_network
// Run: ./social_network

#include <bits/stdc++.h>
using namespace std;

/**
 * Union-Find (Disjoint Set Union) for community detection.
 * Supports union by rank and path compression.
 */
class UnionFind {
private:
    unordered_map<int, int> parent;
    unordered_map<int, int> rankv;

public:
    void add(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rankv[x] = 0;
        }
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        add(a); add(b);
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rankv[ra] < rankv[rb]) {
            parent[ra] = rb;
        } else if (rankv[ra] > rankv[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rankv[ra]++;
        }
    }

    // Build communities: root -> members
    unordered_map<int, vector<int>> getCommunities() {
        unordered_map<int, vector<int>> groups;
        for (auto &p : parent) {
            int node = p.first;
            int root = find(node);
            groups[root].push_back(node);
        }
        return groups;
    }
};

/**
 * Graph representation for the social network.
 * Undirected graph using adjacency list.
 */
class Graph {
private:
    unordered_map<int, unordered_set<int>> adj;

public:
    void addUser(int u) {
        if (adj.find(u) == adj.end()) {
            adj[u] = {};
        }
    }

    void addFriendship(int u, int v) {
        addUser(u); addUser(v);
        adj[u].insert(v);
        adj[v].insert(u);
    }

    bool hasUser(int u) const {
        return adj.find(u) != adj.end();
    }

    const unordered_set<int>& neighbors(int u) const {
        static unordered_set<int> empty;
        auto it = adj.find(u);
        if (it == adj.end()) return empty;
        return it->second;
    }

    vector<int> getUsers() const {
        vector<int> users;
        users.reserve(adj.size());
        for (auto &p : adj) users.push_back(p.first);
        return users;
    }

    // BFS shortest path from src to dst; returns path or empty if unreachable
    vector<int> shortestPath(int src, int dst) const {
        if (!hasUser(src) || !hasUser(dst)) return {};
        unordered_map<int, int> parent;
        queue<int> q;
        unordered_set<int> visited;

        q.push(src);
        visited.insert(src);
        parent[src] = -1;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == dst) break;
            for (int v : neighbors(u)) {
                if (!visited.count(v)) {
                    visited.insert(v);
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (!visited.count(dst)) return {}; // no path

        // Reconstruct path
        vector<int> path;
        for (int cur = dst; cur != -1; cur = parent[cur]) {
            path.push_back(cur);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // Mutual friends count between u and candidate v
    int mutualFriendsCount(int u, int v) const {
        const auto &Nu = neighbors(u);
        const auto &Nv = neighbors(v);
        int count = 0;
        // Iterate smaller set for efficiency
        if (Nu.size() < Nv.size()) {
            for (int x : Nu) if (Nv.count(x)) count++;
        } else {
            for (int y : Nv) if (Nu.count(y)) count++;
        }
        return count;
    }

    // Friend recommendations for user u based on mutual connections
    // Returns top-K candidates sorted by mutual count desc, then by user id asc
    vector<pair<int,int>> recommendFriends(int u, int K = 5) const {
        vector<pair<int,int>> candidates; // (candidate, mutualCount)
        if (!hasUser(u)) return candidates;

        unordered_set<int> excluded = neighbors(u);
        excluded.insert(u); // exclude self and existing friends

        for (int v : getUsers()) {
            if (excluded.count(v)) continue;
            int m = mutualFriendsCount(u, v);
            if (m > 0) candidates.push_back({v, m});
        }

        sort(candidates.begin(), candidates.end(), [](auto &a, auto &b){
            if (a.second != b.second) return a.second > b.second; // mutual desc
            return a.first < b.first; // id asc
        });

        if ((int)candidates.size() > K) candidates.resize(K);
        return candidates;
    }
};

/**
 * SocialNetwork orchestrates Graph + UnionFind features.
 */
class SocialNetwork {
private:
    Graph graph;
    UnionFind uf;

public:
    void addUser(int u) {
        graph.addUser(u);
        uf.add(u);
    }

    void addFriendship(int u, int v) {
        graph.addFriendship(u, v);
        uf.unite(u, v);
    }

    vector<int> shortestPath(int src, int dst) const {
        return graph.shortestPath(src, dst);
    }

    vector<pair<int,int>> recommendFriends(int u, int K = 5) const {
        return graph.recommendFriends(u, K);
    }

    unordered_map<int, vector<int>> communities() {
        return uf.getCommunities();
    }

    vector<int> users() const {
        return graph.getUsers();
    }
};

// ---------- Demo CLI ----------
void printPath(const vector<int>& path) {
    if (path.empty()) {
        cout << "No path found.\n";
        return;
    }
    cout << "Shortest path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n";
}

void printRecommendations(const vector<pair<int,int>>& recs) {
    if (recs.empty()) {
        cout << "No recommendations available.\n";
        return;
    }
    cout << "Friend recommendations (user: mutual_count):\n";
    for (auto &p : recs) {
        cout << "  " << p.first << ": " << p.second << "\n";
    }
}

void printCommunities(unordered_map<int, vector<int>> &groups) {
    cout << "Communities (root -> members):\n";
    for (auto &kv : groups) {
        cout << "  Root " << kv.first << " -> [ ";
        auto members = kv.second;
        sort(members.begin(), members.end());
        for (int m : members) cout << m << " ";
        cout << "]\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SocialNetwork sn;

    // Sample dataset
    // Users: 1..8
    for (int u = 1; u <= 8; ++u) sn.addUser(u);

    // Friendships (undirected edges)
    sn.addFriendship(1, 2);
    sn.addFriendship(1, 3);
    sn.addFriendship(2, 3);
    sn.addFriendship(2, 4);
    sn.addFriendship(3, 5);
    sn.addFriendship(6, 7);
    sn.addFriendship(7, 8);

    cout << "Users: ";
    for (int u : sn.users()) cout << u << " ";
    cout << "\n\n";

    // Shortest path demo
    cout << "[Shortest Path] From 1 to 5:\n";
    auto path = sn.shortestPath(1, 5);
    printPath(path);
    cout << "\n";

    // Recommendations demo
    cout << "[Recommendations] For user 4 (top-5):\n";
    auto recs4 = sn.recommendFriends(4, 5);
    printRecommendations(recs4);
    cout << "\n";

    cout << "[Recommendations] For user 6 (top-5):\n";
    auto recs6 = sn.recommendFriends(6, 5);
    printRecommendations(recs6);
    cout << "\n";

    // Communities demo
    auto groups = sn.communities();
    printCommunities(groups);

    return 0;
}
