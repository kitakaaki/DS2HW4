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
- `7b60f18` 5/19 18:39 update (2026-05-19 18:40:02 +0800) by kitakaaki
<!-- latest-commit-end -->
