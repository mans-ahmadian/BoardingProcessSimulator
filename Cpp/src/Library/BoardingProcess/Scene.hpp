#pragma once


namespace JimaTech::BroadingProcess
{
	class Scene
	{
		int m_numberOfSeatRows = 2;
		int m_numberOfSeatInABlock = 3;
		int m_numberOfSeatBlocks = 2;

	public:
		Scene(int numberOfSeatRows=20,int numberOfSeatInABlock=3,int numberOfSeatBlocks = 2):m_numberOfSeatRows(numberOfSeatRows),m_numberOfSeatInABlock(numberOfSeatInABlock),m_numberOfSeatBlocks(numberOfSeatBlocks)
		{}
		int GetNumberOfSeatRows() { return m_numberOfSeatRows; } 
		int GetNumberOfSeatInABlock() { return m_numberOfSeatInABlock; }
		int GetNumberOfSeatBlocks() { return m_numberOfSeatBlocks; }
		int GetNumberOfSeats(){return m_numberOfSeatRows * m_numberOfSeatInABlock * m_numberOfSeatBlocks;}
		int GetNumberOfAisles() {return GetNumberOfSeatBlocks() - 1;}
		
		TicketInfo GetTicketinfo(int ticketSerialNumber)
		{
			int SeatPerRow = m_numberOfSeatBlocks * m_numberOfSeatInABlock;
			//std::cout << int(ticketSerialNumber / SeatPerRow) << std::endl;
			return TicketInfo(ticketSerialNumber, ticketSerialNumber / SeatPerRow, ticketSerialNumber % SeatPerRow);
		}
	};
}