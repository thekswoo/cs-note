#!/bin/bash
# DOSBox 실습 소스 → cs-note 복사 → GitHub push
set -e

rsync -av --chmod=F644,D755 \
  /mnt/c/SOFTWARE/uCOS-II/EX1_x86L/BC45/SOURCE/ \
  ~/cs-note/2026-2/embedded-software/ucos-ii/EX1/

cd ~/cs-note
git add 2026-2/embedded-software

if git diff --cached --quiet; then
  echo "변경사항 없음"
else
  git commit -m "${1:-embedded-software: update}"
  git push
fi
