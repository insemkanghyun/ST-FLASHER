// FileTransferCheck.h

#ifndef FILE_TRANSFER_CHECK_H
#define FILE_TRANSFER_CHECK_H

#include "main.h" // GPIO 핀 설정을 위해 필요

// LED 상태 초기화 함수
void FileTransferCheck_Init(void);

// 파일 전송 중 상태 업데이트 함수
void FileTransferCheck_UpdateOnWrite(void);

// 파일 전송 완료 상태를 체크하는 함수 (SysTick에서 호출)
void FileTransferCheck_CheckCompletion(void);

// 파일 전송 완료 플래그 확인 함수
uint8_t FileTransferCheck_IsTransferComplete(void);

// 파일 전송 완료 플래그 리셋 함수
void FileTransferCheck_ResetTransferCompleteFlag(void);

void FileTransferCheck(void);

#endif // FILE_TRANSFER_CHECK_H
