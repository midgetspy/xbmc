/*
 *  Copyright (C) 2016-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IMemoryStream.h"

#include <memory>
#include <stdint.h>

namespace KODI
{
namespace RETRO
{
  class CLinearMemoryStream : public IMemoryStream
  {
  public:
    CLinearMemoryStream();

    virtual ~CLinearMemoryStream() = default;

    // partial implementation of IMemoryStream
    virtual void Init(size_t frameSize, uint64_t maxFrameCount) override;
    virtual void Reset() override;
    virtual size_t FrameSize() const override { return m_frameSize; }
    virtual uint64_t MaxFrameCount() const override { return m_maxFrames; }
    virtual void SetMaxFrameCount(uint64_t maxFrameCount) override;
    virtual uint8_t* BeginFrame() override;
    virtual void SubmitFrame() override;
    virtual const uint8_t* CurrentFrame() const override;
    virtual uint64_t FutureFramesAvailable() const override { return 0; }
    virtual uint64_t AdvanceFrames(uint64_t frameCount) override { return 0; }
    virtual uint64_t PastFramesAvailable() const override = 0;
    virtual uint64_t RewindFrames(uint64_t frameCount) override = 0;
    virtual uint64_t GetFrameCounter() const override { return m_currentFrameHistory; }
    virtual void SetFrameCounter(uint64_t frameCount) override { m_currentFrameHistory = frameCount; }

  protected:
    virtual void SubmitFrameInternal() = 0;
    virtual void CullPastFrames(uint64_t frameCount) = 0;

    // Helper function
    uint64_t BufferSize() const;

    size_t m_paddedFrameSize;
    uint64_t m_maxFrames;

    /**
     * Simple double-buffering. After XORing the two states, the next becomes
     * the current, and the current becomes a buffer for the next call to
     * CGameClient::Serialize().
     */
    std::unique_ptr<uint32_t[]> m_currentFrame;
    std::unique_ptr<uint32_t[]> m_nextFrame;
    bool m_bHasCurrentFrame;
    bool m_bHasNextFrame;

    uint64_t m_currentFrameHistory;

  private:
    size_t m_frameSize;
  };
}
}
