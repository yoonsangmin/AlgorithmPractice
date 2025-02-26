﻿#include "PreCompiledHeader.h"

#include "Engine.h"
#include <Windows.h>
#include <iostream>

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <time.h>

#include "Render/ScreenBuffer.h"

// 콘솔 창 메시지 콜백 함수.
BOOL WINAPI MessageProcessor(DWORD message)
{
	switch (message)
	{
	case CTRL_CLOSE_EVENT:
		Engine::Get().QuitGame();
		return true;

	default:
		return false;
	}
}

// 스태틱 변수 초기화.
Engine* Engine::instance = nullptr;

Engine::Engine()
	: quit(false), mainLevel(nullptr), screenSize(60, 25)
{
	// 랜덤 시드 설정.
	srand((unsigned int)time(nullptr));

	// 싱글톤 객체 설정.
	instance = this;

	// 기본 타겟 프레임 속도 설정.
	SetTargetFrameRate(60.0f);

	// 화면 버퍼 초기화.
	// 1. 버퍼 크기 할당.
	imageBuffer = new CHAR_INFO[(screenSize.x) * screenSize.y];

	// 버퍼 초기화.
	ClearImageBuffer();

	// 두 개의 버퍼 생성 (버퍼를 번갈아 사용하기 위해-더블 버퍼링).
	COORD size = { (short)screenSize.x, (short)screenSize.y };
	//renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), size);
	renderTargets[0] = new ScreenBuffer(size);
	renderTargets[1] = new ScreenBuffer(size);

	// 스왑 버퍼.
	Present();

	// 콘솔 창 이벤트 콜백 함수 등록.
	SetConsoleCtrlHandler(MessageProcessor, true);

	// 마우스/윈도우 이벤트 활성화.
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	int flag = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(inputHandle, flag);

	// std::cin/std::cout 연결 끊기.
	std::ios::sync_with_stdio(false);
}

Engine::~Engine()
{
	// 메인 레벨 메모리 해제.
	if (mainLevel != nullptr)
	{
		delete mainLevel;
	}

	// 클리어 버퍼 삭제.
	delete[] imageBuffer;

	// 화면 버퍼 삭제.
	delete renderTargets[0];
	delete renderTargets[1];
}

void Engine::Run()
{
	// 시스템 시계 -> 고해상도 카운터. (10000000).
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 시작 시간 및 이전 시간을 위한 변수.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	int64_t currentTime = time.QuadPart;
	int64_t previousTime = currentTime;

	// Game-Loop.
	while (true)
	{
		// 종료 조건.
		if (quit)
		{
			break;
		}

		// 현재 프레임 시간 저장.
		QueryPerformanceCounter(&time);
		currentTime = time.QuadPart;

		// 프레임 시간 계산.
		float deltaTime = static_cast<float>(currentTime - previousTime) /
			static_cast<float>(frequency.QuadPart);

		// 프레임 확인.
		if (deltaTime >= targetOneFrameTime)
		{
			// 입력 처리 (현재 키/마우스 입력 상태 확인).
			ProcessInput();

			Update(deltaTime);
			Draw();

			// 키 상태 저장.
			SavePreviouseKeyStates();

			// 이전 프레임 시간 저장.
			previousTime = currentTime;

			// 액터 정리 (추가 및 삭제 요청된 액터들 정리).
			if (mainLevel)
			{
				AssignAddRequestedActors();
				mainLevel->ProcessAddedAndDestroyedActor();
			}
		}
	}
}

void Engine::LoadLevel(Level* newLevel)
{
	// 기존 레벨이 있다면 삭제 후 교체.

	// 메인 레벨 설정.
	mainLevel = newLevel;
}

void Engine::AssignAddRequestedActors()
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	// 액터 추가 요청.
	for (const std::shared_ptr<Actor>& actor : addRequestedActors)
	{
		mainLevel->AddActor(actor);
	}

	addRequestedActors.clear();
}

void Engine::DestroyActor(Actor* targetActor)
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	targetActor->Destroy();
}

void Engine::SetCursorType(CursorType cursorType)
{
	GetRenderer()->SetCursorType(cursorType);
}

void Engine::Draw(const Vector2& position, const char* image, Color color)
{
	for (int ix = 0; ix < (int)strlen(image); ++ix)
	{
		int index = (position.y * (screenSize.x)) + position.x + ix;
		imageBuffer[index].Char.AsciiChar = image[ix];
		imageBuffer[index].Attributes = (unsigned long)color;
	}
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
	this->targetFrameRate = targetFrameRate;
	targetOneFrameTime = 1.0f / targetFrameRate;
}

bool Engine::GetKey(int key)
{
	return keyState[key].isKeyDown;
}

bool Engine::GetKeyDown(int key)
{
	return keyState[key].isKeyDown && !keyState[key].wasKeyDown;
}

bool Engine::GetKeyUp(int key)
{
	return !keyState[key].isKeyDown && keyState[key].wasKeyDown;
}

Vector2 Engine::MousePosition() const
{
	return mousePosition;
}

void Engine::QuitGame()
{
	// 종료 플래그 설정.
	quit = true;
}

Engine& Engine::Get()
{
	// 싱글톤 객체 반환.
	return *instance;
}

void Engine::ProcessInput()
{
	static HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD record;
	DWORD events;
	if (PeekConsoleInput(inputHandle, &record, 1, &events) && events > 0)
	{
		if (ReadConsoleInput(inputHandle, &record, 1, &events))
		{
			switch (record.EventType)
			{
			case KEY_EVENT:
			{
				// 키 눌림 상태 업데이트.
				if (record.Event.KeyEvent.bKeyDown)
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = true;
				}
				// 키 눌림 해제 상태 업데이트.
				else
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = false;
				}
			}
			break;

			case MOUSE_EVENT:
			{
				// 마우스 커서 위치 업데이트.
				mousePosition.x = record.Event.MouseEvent.dwMousePosition.X;
				mousePosition.y = record.Event.MouseEvent.dwMousePosition.Y;

				// 마우스 왼쪽 버튼 클릭 상태 업데이트.
				keyState[VK_LBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

				// 마우스 오른쪽 버튼 클릭 상태 업데이트.
				keyState[VK_RBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;

                // 마우스 가운데 버튼 클릭 상태 업데이트.
                keyState[VK_MBUTTON].isKeyDown
                    = (record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) != 0;
			}
			break;

			//		//case WINDOW_BUFFER_SIZE_EVENT:
			//{
			//	char buffer[100];
			//	sprintf_s(buffer, 100, "(%d,%d)", 
			//		record.Event.WindowBufferSizeEvent.dwSize.X, record.Event.WindowBufferSizeEvent.dwSize.Y
			//	);

			//	MessageBoxA(nullptr, buffer, "Test", MB_OK);
			//} break;
			}
		}
	}

	//// 키 입력 테스트.
	//for (int ix = 0; ix < 255; ++ix)
	//{
	//	keyState[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) ? true : false;
	//}
}

void Engine::Update(float deltaTime)
{
	// 레벨 업데이트.
	if (mainLevel != nullptr)
	{
		mainLevel->Update(deltaTime);
	}
}

void Engine::Clear()
{
	ClearImageBuffer();
	//GetRenderer()->Clear();
}

void Engine::Draw()
{
	// 화면 지우기.
	Clear();

	//char buffer[256];
	//snprintf(buffer, 256, "mouse position: (%d,%d)\n", mousePosition.x, mousePosition.y);
	//Draw(Vector2(15, 1), buffer);

	//const char* state = (keyState['a'].isKeyDown) ? "True" : "False";
	//snprintf(buffer, 256, "keyState: %s", state);
	//Draw(Vector2(15, 2), buffer);

	// 레벨 그리기.
	if (mainLevel != nullptr)
	{
		mainLevel->Draw();
	}

	// 백버퍼에 데이터 쓰기.
	GetRenderer()->Draw(imageBuffer);

	// 프론트<->백 버퍼 교환.
	Present();
}

void Engine::Present()
{
	// Swap Buffer.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

void Engine::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyState[ix].wasKeyDown = keyState[ix].isKeyDown;
	}
}

void Engine::ClearImageBuffer()
{
	// 버퍼 덮어쓰기.
	for (int y = 0; y < screenSize.y; ++y)
	{
		// 버퍼 덮어쓰기.
		for (int x = 0; x < screenSize.x; ++x)
		{
			auto& buffer = imageBuffer[y * screenSize.x + x];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
		}
	}
}