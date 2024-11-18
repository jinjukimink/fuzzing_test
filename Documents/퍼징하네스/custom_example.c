#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fuzzer/FuzzedDataProvider.h"

// 메모리 관리 함수
void clean_up(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

// 퍼징 대상 함수 예시 1 (데이터 처리)
static void process_data(const uint8_t *data, size_t size) {
    // 데이터 일부를 사용해 특정 연산 수행
    if (size < 10) return;
    uint32_t header = *(uint32_t *)data;
    uint32_t checksum = *(uint32_t *)(data + 4);
    uint16_t flags = *(uint16_t *)(data + 8);

    // 간단한 데이터 검증 로직
    if (flags & 0x1) {
        printf("Header: %u, Checksum: %u\n", header, checksum);
    }
}

// 퍼징 대상 함수 예시 2 (동적 메모리 활용)
static void dynamic_memory_test(const uint8_t *data, size_t size) {
    if (size == 0) return;

    // 동적 메모리 할당
    void *buffer = malloc(size);
    if (buffer == NULL) {
        perror("malloc");
        return;
    }

    // 데이터 복사 및 간단한 연산 수행
    memcpy(buffer, data, size);
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= ((uint8_t *)buffer)[i];
    }

    printf("Checksum: %x\n", checksum);

    // 메모리 해제
    clean_up(buffer);
}

// 퍼징 메인 함수
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0; // 빈 입력은 무시
    }

    FuzzedDataProvider provider(data, size);

    // 입력 데이터 분할
    size_t split_point = provider.ConsumeIntegralInRange<size_t>(0, size);
    const uint8_t *segment1 = provider.ConsumeBytes<uint8_t>(split_point).data();
    const uint8_t *segment2 = provider.ConsumeBytes<uint8_t>(size - split_point).data();

    // 퍼징 대상 함수 호출
    process_data(segment1, split_point);
    dynamic_memory_test(segment2, size - split_point);

    return 0;
}