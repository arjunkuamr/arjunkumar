
# 📊 Social Network Analysis Tool (C++ + DSA)

A **C++ project** that demonstrates the use of **Data Structures & Algorithms (DSA)** for analyzing social networks.  
This tool models users and friendships as a graph, and provides features like **friend recommendations**, **community detection**, and **shortest path analysis**.

---

## 🚀 Features
- **Friend Recommendation**  
  Suggests new friends based on mutual connections using graph traversal.

- **Community Detection**  
  Uses **Union-Find (Disjoint Set Union)** to detect connected components (communities).

- **Shortest Path Between Users**  
  Implements **Breadth-First Search (BFS)** to find the shortest path between two users.

---

## 🛠️ Tech Stack
- **Language:** C++17  
- **Concepts Used:** Graphs, BFS, Union-Find, Sorting, STL Containers (unordered_map, unordered_set, vector, queue)

---

## 📂 Project Structure


Sample Input/OutputInput (demo)Users: 1..8
Friendships:1-2, 1-3, 2-3, 2-4, 3-5, 6-7, 7-8
OutputUsers: 1 2 3 4 5 6 7 8

[Shortest Path] From 1 to 5:
Shortest path: 1 -> 3 -> 5

[Recommendations] For user 4 (top-5):
Friend recommendations (user: mutual_count):
  5: 1
  1: 1
  3: 1

[Recommendations] For user 6 (top-5):
No recommendations available.

Communities (root -> members):
  Root 1 -> [ 1 2 3 4 5 ]
  Root 6 -> [ 6 7 8 ]
📖 Explanation- Graph Representation:
Users are nodes, friendships are undirected edges.
- Friend Recommendation:
For a given user, candidates are sorted by number of mutual friends.
- Community Detection:
Union-Find groups users into connected components.
- Shortest Path:
BFS finds the minimum number of hops between two users.
🎯 Why This Project?- Demonstrates real-world application of DSA.
- Recruiter-friendly: shows problem-solving, optimization, and graph theory knowledge.
- Clean, modular code with documentation and sample output for GitHub portfolio.
📌 Future Improvements- Add weighted edges (friendship strength).
- Implement visualization (e.g., Graphviz).
- Support dynamic input via file or user commands.
👨‍💻 Author- Arjun — B.Tech CSE (Data Analytics & Data Science)
- Passionate about data analysis, visualization, and building resume-worthy projects.

---




