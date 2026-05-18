# DS2HW4

## Newest Program Features Overview

- Interactive command-line menu with three options: quit, build adjacency lists, and compute connection counts.
- Binary dataset loading from `pairs{N}.bin` with file-existence validation and graceful error messaging.
- Automatic directed graph construction using sender/receiver IDs, including nodes that only appear as receivers.
- Deterministic adjacency-list output to `pairs{N}.adj`, with sender IDs and receiver edges sorted for stable results.
- Reachability analysis using BFS for each sender node that has outgoing edges.
- Connection-count report generation to `pairs{N}.cnt`, including each sender’s total reachable nodes and detailed reachable-node listing.
- Graph reset behavior when rebuilding data to ensure each run starts from a clean in-memory state.

## Latest Commit

<!-- latest-commit-start -->
- `79cfaa9` Merge pull request #2 from kitakaaki/copilot/add-github-action-bot (2026-05-18 22:25:33 +0800) by HUANG YU CHIEN
<!-- latest-commit-end -->
