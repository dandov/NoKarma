/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MESSAGE_H
#define MESSAGE_H
#pragma once
enum MessageType {
	Audio_MessageType,
	HideRender_MessageType,
	InputEvent_Action_MessageType,
	InputEvent_Axis_MessageType,
	InputEvent_State_MessageType,
	InputEvent_Flow_MessageType,
	InputEvent_MappedInput,
	BeatEvent_MessageType,
	BeatAccuracy_MessageType,
	BeatStateChange_MessageType,
	CameraPointMove_MessageType,
	CollisionEvent_AuraBlob_MessageType,
	CollisionEvent_AttackReceived_MessageType,
	StatChangeEvent_AuraBlobDropped_MessageType,
	StatChangeEvent_AttackPerformed_MessageType,
	RewardEvent_MessageType
};

struct MessageData {
	bool handled;
};

#endif
