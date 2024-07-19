// SPDX-FileCopyrightText: 2019-2022 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include "state_wrapper.h"
#include "common/log.h"
#include "common/small_string.h"
#include <cinttypes>
#include <cstring>
Log_SetChannel(StateWrapper);

StateWrapper::StateWrapper(ByteStream* stream, Mode mode, u32 version)
  : m_stream(stream), m_mode(mode), m_version(version)
{
}

StateWrapper::~StateWrapper() = default;

void StateWrapper::DoBytes(void* data, size_t length)
{
  if (m_mode == Mode::Read)
  {
    if (m_error || (m_error |= !m_stream->Read2(data, static_cast<u32>(length))) == true)
      std::memset(data, 0, length);
  }
  else
  {
    if (!m_error)
      m_error |= !m_stream->Write2(data, static_cast<u32>(length));
  }
}

void StateWrapper::DoBytesEx(void* data, size_t length, u32 version_introduced, const void* default_value)
{
  if (m_mode == Mode::Read && m_version < version_introduced)
  {
    std::memcpy(data, default_value, length);
    return;
  }

  DoBytes(data, length);
}

void StateWrapper::Do(bool* value_ptr)
{
  if (m_mode == Mode::Read)
  {
    u8 value = 0;
    if (!m_error)
      m_error |= !m_stream->ReadByte(&value);
    *value_ptr = (value != 0);
  }
  else
  {
    u8 value = static_cast<u8>(*value_ptr);
    if (!m_error)
      m_error |= !m_stream->WriteByte(value);
  }
}

void StateWrapper::Do(std::string* value_ptr)
{
  u32 length = static_cast<u32>(value_ptr->length());
  Do(&length);
  if (m_mode == Mode::Read)
    value_ptr->resize(length);
  DoBytes(&(*value_ptr)[0], length);
  value_ptr->resize(std::strlen(&(*value_ptr)[0]));
}

void StateWrapper::Do(SmallStringBase* value_ptr)
{
  u32 length = static_cast<u32>(value_ptr->length());
  Do(&length);
  if (m_mode == Mode::Read)
    value_ptr->resize(length);
  DoBytes(value_ptr->data(), length);
  value_ptr->update_size();
}

void StateWrapper::Do(std::string_view* value_ptr)
{
  Assert(m_mode == Mode::Write);
  u32 length = static_cast<u32>(value_ptr->length());
  Do(&length);
  DoBytes(const_cast<char*>(value_ptr->data()), length);
}

bool StateWrapper::DoMarker(const char* marker)
{
  SmallString file_value(marker);
  Do(&file_value);
  if (m_error)
    return false;

  if (m_mode == Mode::Write || file_value.equals(marker))
    return true;

  ERROR_LOG("Marker mismatch at offset {}: found '{}' expected '{}'", m_stream->GetPosition(), file_value, marker);
  return false;
}
