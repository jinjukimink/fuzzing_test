#include <inttypes.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "sndfile_fuzz_header.h"

extern "C" {
#include "sndfile.h"
}

// 리소스 정리 함수
static void clean_up(SNDFILE *sndfile) {
    if (sndfile != NULL) {
        sf_close(sndfile);
    }
}

// 데이터 검증 함수
static bool validate_data(const uint8_t *data, size_t size) {
    // 데이터가 최소 크기를 만족하는지 확인
    return (size >= 4 && data != NULL);
}

// sf_command 호출 함수
static void execute_sf_command(SNDFILE *sndfile, FuzzedDataProvider &provider) {
    int cmd = provider.ConsumeIntegral<int>();
    std::vector<uint8_t> cmd_data = provider.ConsumeRemainingBytes<uint8_t>();
    int data_size = cmd_data.size();

    // sf_command 호출
    int result = sf_command(sndfile, cmd, cmd_data.data(), data_size);
    if (result != 0) {
        fprintf(stderr, "sf_command failed: cmd=%d, result=%d, data_size=%d\n", cmd, result, data_size);
    }
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 입력 데이터 검증
    if (!validate_data(data, size)) {
        return 0; // 유효하지 않은 데이터는 무시
    }

    FuzzedDataProvider provider(data, size);

    // 데이터 초기화 및 소비
    SF_INFO sndfile_info;
    VIO_DATA vio_data;
    SF_VIRTUAL_IO vio;
    SNDFILE *sndfile = NULL;

    // sf_init_file 호출
    int init_err = sf_init_file(data, size, &sndfile, &vio_data, &vio, &sndfile_info);
    if (init_err) {
        fprintf(stderr, "sf_init_file failed: init_err=%d\n", init_err);
        clean_up(sndfile);
        return 0;
    }

    // sf_command 실행
    execute_sf_command(sndfile, provider);

    // 리소스 정리
    clean_up(sndfile);

    return 0;
}
