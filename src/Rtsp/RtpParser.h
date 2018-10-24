﻿/*
 * MIT License
 *
 * Copyright (c) 2016 xiongziliang <771730766@qq.com>
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_RTP_RTPPARSER_H_
#define SRC_RTP_RTPPARSER_H_

#include <unordered_map>
#include "Rtsp/Rtsp.h"
#include "Player/Player.h"
#include "Player/PlayerBase.h"
#include "Util/TimeTicker.h"
#include "RTP/RtpCodec.h"

using namespace std;
using namespace ZL::Util;
using namespace ZL::Player;

namespace ZL {
namespace Rtsp {

class RtpParser : public PlayerBase{
public:
	typedef std::shared_ptr<RtpParser> Ptr;
	RtpParser(const string &sdp);
	virtual ~RtpParser(){};

	//返回值：true 代表是i帧第一个rtp包
	bool inputRtp(const RtpPacket::Ptr &rtp);

	float getDuration() const override {
		return m_fDuration;
	}

	/**
	 * 返回是否完成初始化完毕
	 * 由于有些rtsp的sdp不包含sps pps信息
	 * 所以要等待接收到到sps的rtp包后才能完成
	 * @return
	 */
    bool isInited() const override{
        bool ret = true;
        if(ret && _audioTrack){
        	ret = _audioTrack->getTrackType() != TrackInvalid;
        }
        if(ret && _videoTrack){
			ret = _videoTrack->getTrackType() != TrackInvalid;
		}
		return ret;
    }

    vector<Track::Ptr> getTracks() const override;
private:
	inline void onGetAudioTrack(const RtspTrack &audio);
	inline void onGetVideoTrack(const RtspTrack &video);
private:
	float m_fDuration = 0;
	AudioTrack::Ptr _audioTrack;
	VideoTrack::Ptr _videoTrack;
	RtpCodec::Ptr _audioRtpDecoder;
	RtpCodec::Ptr _videoRtpDecoder;
};

} /* namespace Rtsp */
} /* namespace ZL */

#endif /* SRC_RTP_RTPPARSER_H_ */
