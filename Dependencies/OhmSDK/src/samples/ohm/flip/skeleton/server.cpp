/*****************************************************************************

		server.cpp
		Copyright (c) 2012 Ohm Force

*Tab=3***********************************************************************/



#if (defined (__MACH__) && __MACH__)
	#include <Carbon/Carbon.h>
	#define ohm_util_TRACE_DEBUG_FLAG
	#define ohm_archi_EMULATE_LINUX_ON_MACOSX
	#define ohm_sys_NO_DEPENDENCIES
	#define ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES
	#include "ohm/odbg/macosx/assert.cpp"

#elif (defined (__linux__))
	#define ohm_sys_NO_DEPENDENCIES
	#define ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES

#else
	#error Unsupported Operating System

#endif



#define ohm_flip_SERVER
#define ohm_mon_DISABLE_TRACE_FLAG



#include "ohm/archi/def.h"

// lib/ohm/flip
#include "ohm/flip/Args.cpp"
#include "ohm/flip/Blob.cpp"
#include "ohm/flip/Bool.cpp"
#include "ohm/flip/ClassDescManager.cpp"
#include "ohm/flip/ComLayerServerBase.cpp"
#include "ohm/flip/Cue.cpp"
#include "ohm/flip/DocumentServer.cpp"
#include "ohm/flip/EnumDescManager.cpp"
#include "ohm/flip/Err.cpp"
#include "ohm/flip/Float64.cpp"
#include "ohm/flip/Int64.cpp"
#include "ohm/flip/Object.cpp"
#include "ohm/flip/ObjectMold.cpp"
#include "ohm/flip/ObjectMoldData.cpp"
#include "ohm/flip/PasteBoard.cpp"
#include "ohm/flip/SessionManager.cpp"
#include "ohm/flip/SessionManagerMemory.cpp"
#include "ohm/flip/ValidationReportWriter.cpp"
#include "ohm/flip/conv/ConvArray.cpp"
#include "ohm/flip/conv/ConvBlob.cpp"
#include "ohm/flip/conv/ConvBool.cpp"
#include "ohm/flip/conv/ConvCollection.cpp"
#include "ohm/flip/conv/ConvCue.cpp"
#include "ohm/flip/conv/ConvDocument.cpp"
#include "ohm/flip/conv/ConvEnum.cpp"
#include "ohm/flip/conv/ConvFloat64.cpp"
#include "ohm/flip/conv/ConvInt64.cpp"
#include "ohm/flip/conv/ConvObject.cpp"
#include "ohm/flip/conv/ConvObjectRef.cpp"
#include "ohm/flip/private/ArrayMaster.cpp"
#include "ohm/flip/private/BasicType.cpp"
#include "ohm/flip/private/BasicTypesName.cpp"
#include "ohm/flip/private/BinaryKey.cpp"
#include "ohm/flip/private/Codec.cpp"
#include "ohm/flip/private/CollectionMaster.cpp"
#include "ohm/flip/private/DataStreamInput.cpp"
#include "ohm/flip/private/DataStreamOutput.cpp"
#include "ohm/flip/private/DocumentBase.cpp"
#include "ohm/flip/private/DocumentDifferentiator.cpp"
#include "ohm/flip/private/DocumentFormatter.cpp"
#include "ohm/flip/private/DocumentKeywords.cpp"
#include "ohm/flip/private/DocumentParser.cpp"
#include "ohm/flip/private/DocumentReader.cpp"
#include "ohm/flip/private/DocumentWriter.cpp"
#include "ohm/flip/private/EnumMaster.cpp"
#include "ohm/flip/private/IdPtrMapSmall.cpp"
#include "ohm/flip/private/MachineCode.cpp"
#include "ohm/flip/private/MsgRouting.cpp"
#include "ohm/flip/private/ObjectManager.cpp"
#include "ohm/flip/private/ObjectRefMaster.cpp"
#include "ohm/flip/private/ObjectTravelAgency.cpp"
#include "ohm/flip/private/RandomGenerator.cpp"
#include "ohm/flip/private/SeedGenerator.cpp"
#include "ohm/flip/private/TransactionsManager.cpp"
#include "ohm/flip/private/TravelerObjectBackup.cpp"
#include "ohm/flip/private/TravelerObjectCheckSynchronization.cpp"
#include "ohm/flip/private/TravelerObjectCollector.cpp"
#include "ohm/flip/private/TravelerObjectInit.cpp"
#include "ohm/flip/private/TravelerObjectRestore.cpp"
#include "ohm/flip/private/TravelerObjectSynchronize.cpp"
#include "ohm/flip/private/TravelerObjectUnassign.cpp"
#include "ohm/flip/private/TravelerSerialKiller.cpp"
#include "ohm/flip/private/TravelerSlaughterWitness.cpp"
#include "ohm/flip/private/TxDesc.cpp"
#include "ohm/flip/private/TxIdGenerator.cpp"
#include "ohm/flip/private/UserManager.cpp"
#include "ohm/flip/private/VirtualMachine.cpp"
#include "ohm/flip/server/DataMaster.cpp"
#include "ohm/flip/server/IpcData.cpp"
#include "ohm/flip/server/Scribe.cpp"
#include "ohm/flip/server/TxSessionGuard.cpp"


// lib/ohm/math
#include "ohm/math/RndGenMT19937.cpp"


// lib/ohm/odbg
#include "ohm/odbg/LogMgr.cpp"


// lib/ohm/sys
#include "ohm/sys/CritSec.cpp"
#include "ohm/sys/posix/Mutex.cpp"
#include "ohm/sys/posix/ThreadMgr.cpp"

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
	#include "ohm/sys/macho/Interlocked.cpp"
	#include "ohm/sys/carbon/TimerReliable.cpp"

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
	#include "ohm/sys/linux/Interlocked.cpp"
	#include "ohm/sys/linux/TimerReliable.cpp"

#else
	#error Unsupported Operating System
#endif


// lib/ohm/txt
#include "ohm/txt/ascii/ascii_fnc.cpp"
#include "ohm/txt/out/ArgBase.cpp"
#include "ohm/txt/out/ArgFloat.cpp"
#include "ohm/txt/out/ArgInt.cpp"
#include "ohm/txt/out/ArgNumBase.cpp"
#include "ohm/txt/out/ArgString.cpp"
#include "ohm/txt/out/FmtInfo.cpp"
#include "ohm/txt/out/Print.cpp"
#include "ohm/txt/out/PrintInterface.cpp"
#include "ohm/txt/out/PrintNa.cpp"
#include "ohm/txt/utf8/ConvNum.cpp"
#include "ohm/txt/utf8/Codec8.cpp"
#include "ohm/txt/utf8/utf8.cpp"
#include "ohm/txt/unicode/CharData.cpp"


// lib/ohm/util
#include "ohm/util/Base64.cpp"
#include "ohm/util/LogMgr.cpp"
#include "ohm/util/LogLine.cpp"
#include "ohm/util/BinaryStreamInput.cpp"
#include "ohm/util/BinaryStreamOutput.cpp"
#include "ohm/util/BinaryStreamOutputBase.cpp"
#include "ohm/util/BinaryStreamOutputNa.cpp"
#include "ohm/util/Delegate.cpp"


// model
#include "model/ClientData.cpp"
#include "model/fnc.cpp"
#include "model/ModelSignal.cpp"
#include "model/ModelTweaker.cpp"
#include "model/ModelValidator.cpp"
#include "model/ObjectRef.cpp"
#include "model/Pedestrian.cpp"
#include "model/Root.cpp"
#include "model/Selection.cpp"
#include "model/SelectionChangeInfo.cpp"
#include "model/SpacialObject.cpp"
#include "model/Taxi.cpp"
#include "model/ViewInfo.cpp"


// model/conv
#include "model/conv/VersionTree.cpp"


// plex
#include "plex/Angel.cpp"
#include "plex/BufferList.cpp"
#include "plex/Central.cpp"
#include "plex/ClientBase.cpp"
#include "plex/ClientFlip.cpp"
#include "plex/ClientHttp.cpp"
#include "plex/Connection.cpp"
#include "plex/FdEventMgr.cpp"
#include "plex/fnc.cpp"
#include "plex/main.cpp"
#include "plex/Process.cpp"
#include "plex/ProtocolFlip.cpp"
#include "plex/Session.cpp"
#include "plex/Stats.cpp"


// plex/http
#include "plex/http/HttpCentral.cpp"
#include "plex/http/HttpStats.cpp"
#include "plex/http/PageBase.cpp"
#include "plex/http/PageLogs.cpp"
#include "plex/http/PageRestart.cpp"
#include "plex/http/PageSessions.cpp"
#include "plex/http/PageState.cpp"


// plex/sess
#include "plex/sess/SessCentral.cpp"
#include "plex/sess/SessSession.cpp"
#include "plex/sess/Storage.cpp"


// plex/shared
#include "plex/shared/Configuration.cpp"
#include "plex/shared/LogLine.cpp"
#include "plex/shared/LogMgr.cpp"
#include "plex/shared/LogNamespace.cpp"
#include "plex/shared/Metric.cpp"
#include "plex/shared/Metrics.cpp"
#include "plex/shared/ProtocolOsHttp.cpp"
#include "plex/shared/ProtocolOsSess.cpp"
#include "plex/shared/SessionStats.cpp"
#include "plex/shared/StatsData.cpp"
