#!/usr/bin/env bash
# Turn each folder under cores/ into its own GitHub repository.
#
# Usage:  ./scripts/create_repos.sh <github-username> [public|private]
# Needs:  git and the GitHub CLI (gh), logged in with `gh auth login`.
#
# For each core it builds a clean copy in _repos/riscv-<core>-study, adds the
# shared side-channel guide and timing lab, commits, creates the GitHub repo,
# and pushes.
set -euo pipefail

OWNER="${1:?usage: $0 <github-username> [public|private]}"
VISIBILITY="${2:-public}"

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$ROOT/_repos"
mkdir -p "$OUT"

for core_dir in "$ROOT"/cores/*/; do
  core="$(basename "$core_dir")"
  repo="riscv-${core}-study"
  dest="$OUT/$repo"

  echo "== $repo =="
  rm -rf "$dest"
  mkdir -p "$dest/docs"

  cp -r "$core_dir". "$dest"/
  cp "$ROOT/docs/side-channel-guide.md" "$dest/docs/"
  cp -r "$ROOT/docs/labs" "$dest/docs/"

  (
    cd "$dest"
    git init -q -b main
    git add -A
    git commit -q -m "Initial commit: ${core} memory and security study"
    gh repo create "${OWNER}/${repo}" "--${VISIBILITY}" --source=. --push
  )
done

echo "Done. Created 10 repositories under ${OWNER}."
