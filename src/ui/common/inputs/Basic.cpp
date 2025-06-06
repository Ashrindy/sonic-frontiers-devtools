#include "Basic.h"
#include <resources/ManagedMemoryRegistry.h>

static char dummy[1] = { '\0' };

static int StringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<csl::ut::String*>(data->UserData);
		my_str->reserve(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

bool InputText(const char* label, csl::ut::String& str, ImGuiInputTextFlags flags) {
	return ImGui::InputText(label, str.c_str() == nullptr ? dummy : str.c_str(), str.size(), flags | ImGuiInputTextFlags_CallbackResize, StringResizeCallback, &str);
}

struct ManagedResourceString {
	char* str;
	hh::fnd::ManagedResource* resource;
};

static int CStringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto strData = static_cast<ManagedResourceString*>(data->UserData);
		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(strData->resource);

		auto* newStr = new (&allocator) char[data->BufSize];
		strcpy_s(newStr, data->BufSize, strData->str);
		allocator.Free(strData->str);

		strData->str = newStr;
		data->Buf = newStr;
	}
	return 0;
}

bool InputText(const char* label, const char*& str, hh::fnd::ManagedResource* resource, ImGuiInputTextFlags flags)
{
	ManagedResourceString data{ const_cast<char*&>(str), resource };
	bool result = ImGui::InputText(label, const_cast<char*&>(str), strlen(str) + 1, flags | ImGuiInputTextFlags_CallbackResize, CStringResizeCallback, &data);
	str = data.str;
	return result;
}

// This is a completely nonstandard hack around the fact that these things don't have resizeable buffers
// (instead they are always allocated to the exact string length) but it shouldn't cause issues.
class ResizeableVariableString : public csl::ut::VariableString {
public:
	void resize(size_t newSize) {
		size_t allocatorAddr = reinterpret_cast<size_t>(allocatorAndFlags);
		csl::fnd::IAllocator* allocator = allocatorAndFlags == nullptr ? hh::fnd::MemoryRouter::GetModuleAllocator() : reinterpret_cast<csl::fnd::IAllocator*>(allocatorAddr & ~1);

		const char* oldStr = buffer;
		char* newBuffer = (char*)allocator->Alloc(newSize, 1);
		buffer = newBuffer;
		allocatorAndFlags = reinterpret_cast<csl::fnd::IAllocator*>(reinterpret_cast<size_t>(allocator) | 1);

		if (oldStr) {
			strcpy_s(newBuffer, newSize, oldStr);

			if (allocatorAddr & 1)
				allocator->Free((void*)oldStr);
		}
		else {
			newBuffer[0] = '\0';
		}
	}

	size_t size() {
		return buffer == nullptr ? 0 : strlen(buffer);
	}

	char* c_str() {
		return const_cast<char*>(buffer == nullptr ? dummy : buffer);
	}
};

static int VariableStringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<ResizeableVariableString*>(data->UserData);
		//IM_ASSERT(my_str->c_str() == data->Buf);
		my_str->resize(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

bool InputText(const char* label, csl::ut::VariableString& str, ImGuiInputTextFlags flags) {
	auto& rstr = static_cast<ResizeableVariableString&>(str);

	bool edited = ImGui::InputText(label, rstr.c_str(), rstr.size() + 1, flags | ImGuiInputTextFlags_CallbackResize, VariableStringResizeCallback, &rstr);

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* resource = *static_cast<hh::fnd::ManagedResource**>(payload->Data);

			if (!str.GetAllocator())
				str.Set(resource->GetName(), -1, hh::fnd::MemoryRouter::GetModuleAllocator());
			else
				str.Set(resource->GetName());
		}
		if (const ImGuiPayload* path = ImGui::AcceptDragDropPayload("Path")) {
			auto* hPathGoc = *static_cast<hh::path::PathComponent**>(path->Data);

			if (!str.GetAllocator())
				str.Set(hPathGoc->GetName(), -1, hh::fnd::MemoryRouter::GetModuleAllocator());
			else
				str.Set(hPathGoc->GetName());
		}
		ImGui::EndDragDropTarget();
	}

	return edited;
}
