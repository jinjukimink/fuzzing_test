#include <inttypes.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <vector>

#include "fuzzer/FuzzedDataProvider.h"

extern "C" {
#include "sndfile_fuzz_header.h"
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 데이터 크기가 너무 작으면 실행하지 않음
    if (size <= 4) {
        return 0;
    }

    // FuzzedDataProvider로 입력 데이터 초기화
    FuzzedDataProvider stream(data, size);

    SF_INFO sndfile_info;
    VIO_DATA vio_data;
    SF_VIRTUAL_IO vio;
    SNDFILE *sndfile = NULL;

    // 무작위 크기 데이터를 생성
    const size_t sndfile_size = stream.ConsumeIntegralInRange<size_t>(0, size - 1);
    auto sndfile_data_vector = stream.ConsumeBytes<uint8_t>(sndfile_size);

    // 유효성검사 추가로 확인
    if (sndfile_data_vector.empty()) {
        return 0;
    }

    // 벡터 데이터를 배열 포인터로 변환
    const uint8_t *sndfile_data = &sndfile_data_vector[0];

    // 가상 파일 초기화
    int err = sf_init_file(sndfile_data, sndfile_size, &sndfile, &vio_data, &vio, &sndfile_info);
    if (err) {
        return 0;
    }

    // 명령 및 크기 데이터 생성
    int cmd = stream.ConsumeIntegral<int>();
    int data_size = stream.ConsumeIntegralInRange<int>(0, size);

    // 메모리 동적 할당
    void *fuzz_data = malloc(data_size);
    if (fuzz_data == NULL) {
        abort(); // 메모리 할당 실패 시 강제 종료
    }

    // 할당된 메모리에 데이터 소비
    stream.ConsumeData(fuzz_data, data_size);

    // 추가 정수 데이터 소비
    int int_data = stream.ConsumeIntegral<int>();

    // sf_command 호출
    sf_command(sndfile, cmd, fuzz_data, int_data);

    // 메모리 해제
    free(fuzz_data);

    // 리소스 정리
    if (sndfile != NULL) {
        sf_close(sndfile);
    }

    return 0;
}
