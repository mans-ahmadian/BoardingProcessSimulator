#pragma once
#include "TicketInfo.hpp"
#include "PositionInfo.hpp"
#include "Scene.hpp"

namespace JimaTech::BroadingProcess
{
	class Passenger
	{
		TicketInfo m_ticketInfo;
		PositionInfo m_positionInfo;
		int m_handBagDelay = 60;

	public:
		Passenger(const TicketInfo& ticketInfo) :m_ticketInfo(ticketInfo)
		{

		}
		Passenger()
		{

		}
		void SetHandBagDelay(int value)
		{
			m_handBagDelay = value;
		}
		void GivePassengerTicket(TicketInfo& ticketInfo)
		{
			m_ticketInfo = ticketInfo;
		}
		TicketInfo* GetTicketInfo()
		{
			return &m_ticketInfo;
		}
		PositionInfo* GetPositionInfo()
		{
			return &m_positionInfo;
		}
		void MoveToAisle(int aisleNo)
		{
			m_positionInfo.m_positionArea = PositionArea::IN_AISLE;
			m_positionInfo.m_aislePositionInfo.m_aisleNo = aisleNo;
			MoveInAisle(0);
		}
		void MoveInAisle(int inAsilePosition)
		{
			if (m_positionInfo.m_positionArea != PositionArea::IN_AISLE)
			{
				throw new std::exception("Passenger moves in aisle when he is not in an aisle yet");
			}
			m_positionInfo.m_aislePositionInfo.m_inAislePosition = inAsilePosition;
		}
		bool IsNextToSeat()
		{
			if (m_positionInfo.m_aislePositionInfo.m_inAislePosition == m_ticketInfo.m_seatRow)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		void  MoveInAisle()
		{
			MoveInAisle(m_positionInfo.m_aislePositionInfo.m_inAislePosition + 1);
		}
		void PutBag()
		{
			if (m_handBagDelay > 0)
			{
				m_handBagDelay--;
			}
			else
			{
				m_positionInfo.m_positionArea = PositionArea::IN_SEAT;
			}
		}

	};

	class Passengers
	{
		std::vector<JimaTech::BroadingProcess::Passenger> m_passengers;
		Scene* m_scene;
		int m_topOfTheQueue = 0;
	public:
		Passengers(int numberOfPassengers, Scene* scene) :m_scene(scene)
		{
			//m_passengers.resize(numberOfPassengers);
			for (int i = 0; i < numberOfPassengers; i++)
			{
				JimaTech::BroadingProcess::Passenger p;
				p.GetPositionInfo()->PutInQueue();
				m_passengers.push_back(p);
			}
		}
		int GetNumberOfPassengers() const
		{
			return m_passengers.size();
		}
		std::vector<JimaTech::BroadingProcess::Passenger>& GetPassengersList()
		{
			return m_passengers;
		}
		void ShufflePassengers()
		{
			// randomly pass the gate:
			std::random_device rd;
					
			//std::mt19937 generator(1234);
			std::mt19937 generator(rd());
			std::shuffle(m_passengers.begin(), m_passengers.end(), generator);
		}
		bool MoveAPassengerToAisle(int aisleNumber)
		{
			if (m_topOfTheQueue < m_passengers.size())
			{
				m_passengers[m_topOfTheQueue++].MoveToAisle(aisleNumber);
				return true;
			}
			return false;
		}

	};
}