# linux-lib-patchwork

Patch collection and reproduction scripts for the Linux kernel `lib/` directory.

## Layout

- `pending/`: Patches currently under review or development.
- `upstreamed/`: Patches accepted into a parent tree.
- `reproducers/`: Scripts and logs for verifying bugs.

## Current Patches

### Pending
- `0000-cover-letter-fsmids.patch`: Fixes to a IDS evasion
- `0001-lib-alloc_tag-allocate-early-page-tags-array-dynamic.patch`: lib/alloc_tag: allocate early page tags array dynamically
- `0001-lib-btree-optimize-btree_merge-to-avoid-three-tree-w.patch`: lib/btree: optimize btree_merge to avoid three tree walks
- `0001-lib-decompress-track-total-bytes-consumed-with-fill-.patch`: lib/decompress: track total bytes consumed with fill function
- `0001-lib-ts_fsm-Fix-text-search-evasion-on-block-boundari.patch`: lib/ts_fsm: Fix text search evasion on block boundaries
- `0002-lib-ts_fsm-Remove-overeager-end-of-data-check.patch`: lib/ts_fsm: Remove overeager end-of-data check
- `debugobjects_null_deref.patch`: debugobjects: fix NULL obj_cache dereference in
- `idr_leak_error_path.patch`: lib/idr: fix memory leak on error path
- `idr_leak_inline_alloc.patch`: lib/idr: fix memory leak on inline allocation
- `ts_fsm_alloc_overflow.patch`: lib/ts_fsm: fix integer overflow in allocation size

### Upstreamed
- `bm_overflow.patch`: lib/ts_bm: fix integer overflow in pattern length calculation
- `kmp_overflow.patch`: lib/ts_kmp: fix integer overflow in pattern length

## Reproducers
- `repro_iptables.sh`
- `results.txt`
