#ifndef FMOD_AUDIO_EFFECT_CAPTURE_H
#define FMOD_AUDIO_EFFECT_CAPTURE_H

#include "dsp/fmod_audio_effect.h"
#include <mutex>
#include <atomic>
#include <vector>

namespace godot {
	// 简单的环形缓冲区实现
	class AudioRingBuffer {
	private:
		std::vector<float> buffer;
		size_t write_pos = 0;
		size_t read_pos = 0;
		size_t size = 0;
		
	public:
		void resize(size_t p_size) {
			buffer.resize(p_size);
			size = p_size;
			write_pos = 0;
			read_pos = 0;
		}
		
		void clear() {
			write_pos = 0;
			read_pos = 0;
		}
		
		// 返回可写入的空间
		size_t space_left() const {
			if (write_pos >= read_pos) {
				return size - (write_pos - read_pos) - 1;
			} else {
				return (read_pos - write_pos) - 1;
			}
		}
		
		// 返回可读取的数据量
		size_t data_left() const {
			if (write_pos >= read_pos) {
				return write_pos - read_pos;
			} else {
				return size - (read_pos - write_pos);
			}
		}
		
		// 写入数据，返回实际写入的数量
		size_t write(const float* data, size_t count) {
			size_t to_write = MIN(count, space_left());
			for (size_t i = 0; i < to_write; i++) {
				buffer[write_pos] = data[i];
				write_pos = (write_pos + 1) % size;
			}
			return to_write;
		}
		
		// 读取数据，返回实际读取的数量
		size_t read(float* data, size_t count) {
			size_t to_read = MIN(count, data_left());
			for (size_t i = 0; i < to_read; i++) {
				data[i] = buffer[read_pos];
				read_pos = (read_pos + 1) % size;
			}
			return to_read;
		}
	};
	
	// 捕获 DSP 特定的状态数据
	struct CaptureDSPState {
		AudioRingBuffer* ring_buffer = nullptr;             // 指向环形缓冲区
		std::mutex* buffer_mutex = nullptr;                 // 缓冲区互斥锁
		std::atomic<int64_t>* pushed_frames = nullptr;      // 总推送帧数
		std::atomic<int64_t>* discarded_frames = nullptr;   // 丢弃帧数
		int channels = 2;                                   // 声道数
		
		CaptureDSPState() = default;
	};
	
	class FmodAudioEffectCapture : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectCapture, FmodAudioEffect)

	protected:
		static void _bind_methods();
		
		// 重写基类虚函数
		virtual void _on_dsp_create(FMOD_DSP_STATE* dsp_state) override;
		virtual void _on_dsp_process(FMOD_DSP_STATE* dsp_state,
			unsigned int length,
			const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
			FMOD_DSP_BUFFER_ARRAY* outbufferarray,
			FMOD_BOOL inputsidle,
			FMOD_DSP_PROCESS_OPERATION op) override;
		virtual void _on_dsp_release(FMOD_DSP_STATE* dsp_state) override;

	private:
		double buffer_length = 1.0;                         // 捕获缓冲区的长度，单位为秒
		int buffer_size = 0;                                // 缓冲区大小（采样数）
		AudioRingBuffer ring_buffer;                        // 环形缓冲区（交错存储：L,R,L,R...）
		mutable std::mutex buffer_mutex;                    // 缓冲区互斥锁
		
		std::atomic<int64_t> pushed_frames{0};              // 总推送帧数
		std::atomic<int64_t> discarded_frames{0};           // 丢弃帧数
		
		bool dsp_created = false;                           // DSP 是否已创建
		
		// 更新缓冲区大小
		void _update_buffer_size();

	public:
		FmodAudioEffectCapture();
		virtual ~FmodAudioEffectCapture();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual void remove_from_bus(Ref<FmodChannelGroup> p_bus) override;

		bool can_get_buffer(const int64_t frames) const;                        // 检查是否可以获取指定数量的帧
		void clear_buffer();                                                    // 清空内部环形缓冲
		PackedVector2Array get_buffer(const int64_t frames);                    // 从内部环形缓冲区获取指定数量的帧
		int64_t get_discarded_frames() const;                                   // 返回由于缓冲区满而丢弃的音频帧数量
		int64_t get_frames_available() const;                                   // 返回可使用 get_buffer() 读取的帧数
		int64_t get_pushed_frames() const;                                      // 返回从音频总线插入的音频帧数量

		void set_buffer_length(double p_buffer_length);                         // 设置内部环形缓冲区的长度，单位为秒
		double get_buffer_length() const;                                       // 获取内部环形缓冲区的长度，单位为秒
	};
}

#endif // !FMOD_AUDIO_EFFECT_CAPTURE_H
