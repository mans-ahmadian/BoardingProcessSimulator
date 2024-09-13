#pragma once
#include <vector>

#include "BoardingProcess\Passenger.hpp"
#include "BoardingProcess\Scene.hpp"

namespace JimaTech::BroadingProcess
{
	enum class AislePostionStatus
	{
		EMPTY,
		Occupied,
	};
	class Simulate
	{
		Scene* m_scene;
		Passengers* m_passengers;
		bool m_isAnyChang = true;
		std::vector<std::vector<AislePostionStatus>> m_aislePositions;
	public:
		Simulate(Scene* scene, Passengers* passengers) : m_scene(scene), m_passengers(passengers)
		{
			m_aislePositions.resize(m_scene->GetNumberOfAisles());
			for (int i = 0; i < m_scene->GetNumberOfAisles(); i++)
			{
				m_aislePositions[i].resize(m_scene->GetNumberOfSeatRows());
			}
		}
		void MoveInAisle()
		{
			m_isAnyChang = false;
			// reset all aisle position to empty
			for (auto& aisleInRow : m_aislePositions) {

				for (auto& oneAislePosition : aisleInRow)
				{
					oneAislePosition = AislePostionStatus::EMPTY;
				}
			}
			//find filled aisle positions
			for (auto& p : m_passengers->GetPassengersList())
			{
				switch (p.GetPositionInfo()->GetPositionArea())
				{
				case PositionArea::IN_QUEUE:

					break;
				case PositionArea::IN_AISLE:
				case PositionArea::IN_BAGTO:
				{
					m_aislePositions[p.GetPositionInfo()->GetAislePositionInfo().m_aisleNo][p.GetPositionInfo()->GetAislePositionInfo().m_inAislePosition] = AislePostionStatus::Occupied;

					break;
				}
				case PositionArea::IN_SEAT:
					break;
				default:
					throw std::exception("Unknown area in MoveInAisle function");
					break;
				}
			}
			// move passengers in aisle
			for (auto& p : m_passengers->GetPassengersList())
			{
				switch (p.GetPositionInfo()->GetPositionArea())
				{

				case PositionArea::IN_AISLE:
				{
					if (p.IsNextToSeat())
					{
						p.GetPositionInfo()->m_positionArea = PositionArea::IN_BAGTO;
						break;
					}
					if (CanMoveToNextPositionInAisle(p))
					{
						p.MoveInAisle();
					}
					break;
				}
				case PositionArea::IN_BAGTO:
				{
					p.PutBag();
					break;
				}
				}
			}

			for (int aisleNo = 0; aisleNo < m_aislePositions.size(); aisleNo++)
			{
				if (m_aislePositions[aisleNo][0] == AislePostionStatus::EMPTY)
				{
					m_passengers->MoveAPassengerToAisle(aisleNo);

				}
			}

		}

		bool CanMoveToNextPositionInAisle(PositionInfo& positionInfo)
		{
			if (positionInfo.GetAislePositionInfo().m_inAislePosition + 1 < m_scene->GetNumberOfSeatRows())
			{
				return (m_aislePositions[positionInfo.GetAislePositionInfo().m_aisleNo][positionInfo.GetAislePositionInfo().m_inAislePosition + 1] == AislePostionStatus::EMPTY);
			}
			else
			{
				return false;
			}
		}
		bool CanMoveToNextPositionInAisle(Passenger& Passenger)
		{
			return CanMoveToNextPositionInAisle(*Passenger.GetPositionInfo());
		}

		bool  IsAllSeated()
		{
			for (auto& p : m_passengers->GetPassengersList())
			{
				if (p.GetPositionInfo()->GetPositionArea() != PositionArea::IN_SEAT)
				{
					return false;
				}
			}
			return true;
		}
	private:

	};

}
