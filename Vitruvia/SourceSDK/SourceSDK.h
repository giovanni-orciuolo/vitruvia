/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ / 
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \ 
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once

// Classes

#include "Classes\CRC\CRC.h"
#include "Classes\Maths\Quaternion.h"
#include "Classes\Maths\Vector.h"
#include "Classes\Maths\Maths.h"
#include "Classes\MD5\MD5.h"
#include "Classes\Recv\ClientClass.h"
#include "Classes\Recv\Recv.h"
#include "Classes\BSPFlags.h"
#include "Classes\CGlobalVarsBase.h"
#include "Classes\CGlowObject.h"
#include "Classes\CHandle.h"
#include "Classes\CInput.h"
#include "Classes\Color.h"
#include "Classes\CUtlMemory.h"
#include "Classes\CUtlVector.h"
#include "Classes\CUtlStack.h"
#include "Classes\KeyValues.h"
#include "Classes\PlayerInfo.h"
#include "Classes\CClientState.h"
#include "Classes\CEffectDispatch.h"
#include "Classes\CHud.h"
#include "Classes\Buffer.h"
#include "Classes\DataMap.h"
#include "Classes\CCSGOPlayerAnimState.h"
#include "Classes\CRender.h"

// Interfaces

#include "Interfaces\Client\IBaseClientDLL.h"
#include "Interfaces\Client\IClientEntity.h"
#include "Interfaces\Client\IClientEntityList.h"
#include "Interfaces\Client\ICVar.h"
#include "Interfaces\Client\IGameMovement.h"
#include "Interfaces\Client\IPrediction.h"
#include "Interfaces\Client\IClientMode.h"
#include "Interfaces\Client\IViewRenderBeams.h"
#include "Interfaces\Engine\ICollideable.h"
#include "Interfaces\Engine\IEngineTrace.h"
#include "Interfaces\Engine\IGameEvent.h"
#include "Interfaces\Engine\INetChannelInfo.h"
#include "Interfaces\Engine\IVDebugOverlay.h"
#include "Interfaces\Engine\IVEngineClient.h"
#include "Interfaces\Engine\IVModelCache.h"
#include "Interfaces\Engine\IVModelInfo.h"
#include "Interfaces\Engine\IVModelRender.h"
#include "Interfaces\Engine\IVPhysics.h"
#include "Interfaces\Engine\IVRenderView.h"
#include "Interfaces\Engine\INetworkStringTable.h"
#include "Interfaces\Engine\IEngineSound.h"
#include "Interfaces\InputSystem\IInputSystem.h"
#include "Interfaces\MaterialSystem\IMaterial.h"
#include "Interfaces\MaterialSystem\IMaterialSystem.h"
#include "Interfaces\Localize\ILocalize.h"
#include "Interfaces\Steam\ISteamHTTP.h"
#include "Interfaces\Surface\IPanel.h"
#include "Interfaces\Surface\ISurface.h"