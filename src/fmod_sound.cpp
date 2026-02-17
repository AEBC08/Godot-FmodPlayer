#include "fmod_sound.h"

namespace godot {
	void FmodSound::_bind_methods() {
		ClassDB::bind_static_method("FmodSound", D_METHOD("load_from_file", "path"), &FmodSound::load_from_file);
		ClassDB::bind_method(D_METHOD("get_name"), &FmodSound::get_name);
		ClassDB::bind_method(D_METHOD("get_format"), &FmodSound::get_format);
		ClassDB::bind_method(D_METHOD("get_length"), &FmodSound::get_length);
		ClassDB::bind_method(D_METHOD("get_num_tags"), &FmodSound::get_num_tags);
		ClassDB::bind_method(D_METHOD("get_tag", "index", "name"), &FmodSound::get_tag);
	}

	FmodSound::FmodSound() {
		
	}

	FmodSound::~FmodSound() {
		if (sound) {
			sound = nullptr;
		}
	}

	Ref<FmodSound> FmodSound::load_from_file(const String& p_path) {
		return FmodServer::get_main_system()->create_sound_from_file(p_path, FmodSystem::MODE_DEFAULT);
	}

	String FmodSound::get_name() const {
		ERR_FAIL_COND_V(!sound, String());
		char name[512] = { 0 };
		FMOD_ERR_CHECK(sound->getName(name, 512));
		return String::utf8(name);
	}

	Dictionary FmodSound::get_format() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;
		FMOD_ERR_CHECK_V(sound->getFormat(&type, &format, &channels, &bits), Dictionary());
		Dictionary info;
		info["type"] = (int)type;
		info["format"] = (int)format;
		info["channels"] = channels;
		info["bits"] = bits;
		return info;
	}

	double FmodSound::get_length() const {
		unsigned int length = 0;
        FMOD_ERR_CHECK(sound->getLength(&length, FMOD_TIMEUNIT_MS));
        return (double)length / 1000.0;
	}

	Dictionary FmodSound::get_num_tags() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		int numtags = 0;
		int numtagsupdated = 0;
		FMOD_ERR_CHECK_V(sound->getNumTags(&numtags, &numtagsupdated), Dictionary());
		Dictionary result;
		result["num_tags"] = numtags;
		result["num_tags_updated"] = numtagsupdated;
		return result;
	}

	Dictionary FmodSound::get_tag(const int index, const String& name) const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		const char* char_ptr = name.utf8().get_data();
		FMOD_TAG tag = {};
		FMOD_ERR_CHECK_V(sound->getTag(char_ptr, index, &tag), Dictionary());
		Dictionary info;
		info["type"] = (int)tag.type;
		info["datatype"] = (int)tag.datatype;
		info["name"] = String::utf8(tag.name);
		switch (tag.datatype) {
		case FMOD_TAGDATATYPE_BINARY: {
			// void* → PackedByteArray
			PackedByteArray data;
			data.resize(tag.datalen);
			if (tag.data != nullptr && tag.datalen > 0) {
				memcpy(data.ptrw(), tag.data, tag.datalen);
			}
			info["data"] = data;
		} break;

		case FMOD_TAGDATATYPE_INT: {
			// void* → int (根据文档，可能是 int 或 long)
			if (tag.datalen == sizeof(int)) {
				info["data"] = *(int*)tag.data;
			}
			else if (tag.datalen == sizeof(long)) {
				info["data"] = (int)(*(long*)tag.data);
			}
		} break;

		case FMOD_TAGDATATYPE_FLOAT: {
			// void* → float
			if (tag.datalen == sizeof(float)) {
				info["data"] = *(float*)tag.data;
			}
			else if (tag.datalen == sizeof(double)) {
				info["data"] = (float)(*(double*)tag.data);
			}
		} break;

		case FMOD_TAGDATATYPE_STRING: {
			// void* → String (UTF-8 字符串)
			if (tag.data != nullptr && tag.datalen > 0) {
				// 确保字符串以 null 结尾
				const char* str_data = static_cast<const char*>(tag.data);
				// 使用 UTF-8 转换
				info["data"] = String::utf8(str_data, tag.datalen);
			}
			else {
				info["data"] = String();
			}
		} break;

		case FMOD_TAGDATATYPE_STRING_UTF16: {
			// void* → String (UTF-16 字符串)
			if (tag.data != nullptr && tag.datalen > 0) {
				// UTF-16 需要特殊处理
				const char16_t* utf16_data = static_cast<const char16_t*>(tag.data);
				int len = tag.datalen / sizeof(char16_t);
				info["data"] = String::utf16(utf16_data, len);
			}
			else {
				info["data"] = String();
			}
			break;
		}

		case FMOD_TAGDATATYPE_STRING_UTF16BE: {
			// UTF-16BE (大端序) - 需要转换
			if (tag.data != nullptr && tag.datalen > 0) {
				// 简单处理：先转为 UTF-8
				const uint8_t* bytes = static_cast<const uint8_t*>(tag.data);
				// 这里需要实现 UTF-16BE 到 UTF-8 的转换
				// 为简化，先存为二进制
				PackedByteArray data;
				data.resize(tag.datalen);
				memcpy(data.ptrw(), bytes, tag.datalen);
				info["data"] = data;
			}
			break;
		}

		case FMOD_TAGDATATYPE_STRING_UTF8: {
			// void* → String (明确指定 UTF-8)
			if (tag.data != nullptr && tag.datalen > 0) {
				info["data"] = String::utf8(static_cast<const char*>(tag.data), tag.datalen);
			}
			else {
				info["data"] = String();
			}
			break;
		}
		}
		info["data_len"] = tag.datalen;
		info["updated"] = tag.updated == 1;
		return info;
	}
}