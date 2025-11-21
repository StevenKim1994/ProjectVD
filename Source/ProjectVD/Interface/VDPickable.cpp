// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/VDPickable.h"

// Add default functionality here for any IVDIPickable functions that are not pure virtual.

bool IVDPickable::CanBePicked()
{
	// DESC :: 실제 구현은 이 인터페이스를 구현하는 클래스에서 작성
    return false;
}

void IVDPickable::OnPicked(AActor* Picker)
{
	// DESC :: 실제 구현은 이 인터페이스를 구현하는 클래스에서 작성
}
