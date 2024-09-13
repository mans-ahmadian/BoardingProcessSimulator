#pragma once
#include <string>

namespace JimaTech::BroadingProcess
{
	struct TicketInfo
	{
		int m_ticketSerialNumber;
		int m_seatRow;
		char m_seatColChar;
		int m_seatCol;
		std::string m_ticketNo;
	public:
		TicketInfo(int ticketSerialNumber, int seatRow, char seatCol) :m_ticketSerialNumber(ticketSerialNumber), m_seatRow(seatRow), m_seatCol(seatCol)
		{
			UpdateTicketNo();
		}
		TicketInfo() :TicketInfo(-1,-1,-1)
		{

		}
		std::string GetTicketNo()
		{
			return m_ticketNo;
		}
		void SetTicketInfo(int ticketSerialNumber, int seatRow, char seatCol)
		{
			m_ticketSerialNumber = ticketSerialNumber;
			m_seatRow = seatRow;
			m_seatCol = seatCol;
			UpdateTicketNo();
		}
	private:
		void UpdateTicketNo()
		{
			m_seatColChar = 'A' + m_seatCol;
			m_ticketNo = std::to_string(m_seatRow + 1) + m_seatColChar;
		}
	};
}