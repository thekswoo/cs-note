#!/bin/bash
# DOSBox 실습 소스 → cs-note 동기화
rsync -av --chmod=F644,D755 \
  /mnt/c/SOFTWARE/uCOS-II/EX1_x86L/BC45/SOURCE/ \
  ~/cs-note/2026-2/embedded-software/ucos-ii/EX1/
