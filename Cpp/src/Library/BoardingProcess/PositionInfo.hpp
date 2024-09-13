#pragma once


namespace JimaTech::BroadingProcess
{
	struct AislePositionInfo
	{
	
		int m_aisleNo;
		int m_inAislePosition;
	};
	enum class PositionArea
	{
		IN_QUEUE,
		IN_AISLE,
		IN_BAGTO,
		IN_SEAT,
	};
	struct PositionInfo
	{		
		PositionArea m_positionArea;
		AislePositionInfo m_aislePositionInfo;
		int SeatRowNumber;
		int SeatColNumber;
	public:
		
		void PutInQueue()
		{
			m_positionArea = PositionArea::IN_QUEUE;
			m_aislePositionInfo.m_aisleNo=-1;
			m_aislePositionInfo.m_inAislePosition=-1;
			SeatRowNumber=-1;
			SeatColNumber=-1;
		}
		
		PositionArea GetPositionArea() const
		{
			return m_positionArea;
		}
		AislePositionInfo GetAislePositionInfo() const
		{
			return m_aislePositionInfo;
		}


	};
}