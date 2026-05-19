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
- `397602c` Fix cin infinite loop bug (2026-05-19 17:24:56 +0800) by kitakaaki
<!-- latest-commit-end -->
