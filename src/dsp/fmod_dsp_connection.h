#ifndef FMOD_DSP_CONNECTION_H
#define FMOD_DSP_CONNECTION_H

#include "core/fmod_system.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
	class FmodDSP;

	class FmodDSPConnection : public RefCounted {
		GDCLASS(FmodDSPConnection, RefCounted)

	private:

	protected:
		static void _bind_methods();

	public:
		FmodDSPConnection();
		~FmodDSPConnection();

		enum FmodDSPConnectionType {
			DSPCONNECTION_TYPE_STANDARD,
			DSPCONNECTION_TYPE_SIDECHAIN,
			DSPCONNECTION_TYPE_SEND,
			DSPCONNECTION_TYPE_SEND_SIDECHAIN,
			DSPCONNECTION_TYPE_PREALLOCATED,

			DSPCONNECTION_TYPE_MAX,
			DSPCONNECTION_TYPE_FORCEINT = 65536
		};

		FMOD::DSPConnection* dsp_connection = nullptr;

		void setup(FMOD::DSPConnection* p_connection);

		// 混合性质
		void set_mix(const float volume_db);									// 设置连接的音量刻度
		float get_mix() const;													// 获取连接的音量刻度

		void set_mix_matrix(
			const PackedFloat32Array& matrix,
			const int outchannels,
			const int inchannels,
			const int inchannel_hop = 0
		);																		// 设置一个二维声像矩阵，将信号从输入通道（列）映射到输出扬声器（行）
		PackedFloat32Array get_mix_matrix(
			const int outchannels,
			const int inchannels,
			const int inchannel_hop
		) const;																// 获取一个二维声像矩阵，将信号从输入通道（列）映射到输出扬声器（行）
	
		// 概述
		Ref<FmodDSP> get_input() const;											// 获取连接的输入 DSP 单元
		Ref<FmodDSP> get_output() const;										// 获取连接的输出 DSP 单元
		FmodDSPConnectionType get_type() const;									// 获取两个 DSP 单元之间的连接类型
	};
}

VARIANT_ENUM_CAST(FmodDSPConnection::FmodDSPConnectionType);

#endif // !FMOD_DSP_CONNECTION_H

