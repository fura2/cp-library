#!/usr/bin/env bash
set -euo pipefail

root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "$root"

if [[ ${1:-} == --help ]]; then
  echo 'Usage: bash tests/run.sh [path-substring ...]'
  echo 'Example: CXX=g++-15 bash tests/run.sh data_structure mint'
  exit 0
fi

if [[ -n ${CXX:-} ]]; then
  compiler=$CXX
elif command -v g++-15 >/dev/null 2>&1; then
  compiler=g++-15
else
  compiler=g++
fi
"$compiler" --version

build_dir=$(mktemp -d "$root/tests/.build.XXXXXX")
trap 'rm -rf "$build_dir"' EXIT
ulimit -c 0
flags=(-std=gnu++23 -Wall -Wextra -Werror -O2 -UNDEBUG -I "$root")
passed=0
failed=0

run_case() {
  local source=$1 variant=$2
  shift 2
  local label="$source${variant:+ ($variant)}"
  local fixture="${source%.test.cpp}${variant:+.$variant}"
  local input=/dev/null expected_out=/dev/null expected_err=/dev/null
  [[ ! -f "$fixture.in" ]] || input="$fixture.in"
  [[ ! -f "$fixture.out" ]] || expected_out="$fixture.out"
  [[ ! -f "$fixture.err" ]] || expected_err="$fixture.err"

  echo "[BUILD] $label"
  if ! "$compiler" "${flags[@]}" "$@" "$source" -o "$build_dir/test"; then
    echo "[FAIL] $label (compile)"
    failed=$((failed + 1))
    return
  fi
  if ! "$build_dir/test" < "$input" > "$build_dir/stdout" 2> "$build_dir/stderr"; then
    cat "$build_dir/stdout" "$build_dir/stderr"
    echo "[FAIL] $label (run)"
    failed=$((failed + 1))
    return
  fi
  local mismatch=0
  diff -u "$expected_out" "$build_dir/stdout" || mismatch=1
  diff -u "$expected_err" "$build_dir/stderr" || mismatch=1
  if [[ $mismatch == 1 ]]; then
    echo "[FAIL] $label (output)"
    failed=$((failed + 1))
  else
    echo "[PASS] $label"
    passed=$((passed + 1))
  fi
}

while IFS= read -r source; do
  selected=0
  if [[ $# == 0 ]]; then
    selected=1
  else
    for filter in "$@"; do
      filter=${filter#"$root/"}
      filter=${filter#./}
      [[ $source != *"$filter"* ]] || selected=1
    done
  fi
  [[ $selected == 1 ]] || continue

  run_case "$source" ''
  case "$source" in
    tests/template/template.test.cpp | tests/template/io.test.cpp)
      run_case "$source" local -DLOCAL
      ;;
    tests/template/main.test.cpp)
      run_case "$source" local -DLOCAL
      run_case "$source" multi -DMULTI_TESTCASE
      run_case "$source" multi_local -DMULTI_TESTCASE -DLOCAL
      ;;
  esac
done < <(find tests -name '*.test.cpp' -type f | LC_ALL=C sort)

echo "Results: $passed passed, $failed failed"
if [[ $((passed + failed)) == 0 ]]; then
  echo 'No tests matched.' >&2
  exit 1
fi
[[ $failed == 0 ]]
