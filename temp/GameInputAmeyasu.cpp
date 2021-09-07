// https://github.com/TraiasChannelServer/GameHinagata/blob/master/GameInput.cpp

char GameInput::Buf1[256];
char GameInput::Buf2[256];
char* GameInput::BufPtrNow = Buf1;
char* GameInput::BufPtrPrev = Buf2;

void GameInput::CheckInput() {
	char* Tmp = BufPtrNow;
	BufPtrNow = BufPtrPrev;
	BufPtrPrev = Tmp;
	
	GetHitKeyStateAll(BufPtrNow);
}

bool GameInput::IsKeyPress(int key) {
	return (BufPtrNow[key]) & (~BufPtrPrev[key]);
}
bool GameInput::IsKeyRelease(int key) {
	return (~BufPtrNow[key]) & (BufPtrPrev[key]);
}


