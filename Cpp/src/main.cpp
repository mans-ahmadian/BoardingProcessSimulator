#include <string>
#include <random>
#include <vector>
#include <iostream>

#include "BoardingProcess\SceneWriter.hpp"
#include "Tools\FilesystemTools.h"

#include "BoardingProcess\Passenger.hpp"
#include "BoardingProcess\SceneWriter.hpp"
#include "BoardingProcess\Scene.hpp"
#include "BoardingProcess\Simulate.hpp"



void main()
{
	auto outputFileName = JimaTech::Tools::GetApplicationPath() / "timedata.csv";
	std::ofstream myfile;
	myfile.open(outputFileName.string());
	for (int i = 0; i < 1000; i++)
	{
		try {
			std::string outputDir = (JimaTech::Tools::GetApplicationPath() / ("output"+std::to_string(i))).string();

			// create objects
			// std::vector<JimaTech::BroadingProcess::Passenger> passengers;
			JimaTech::BroadingProcess::Scene scene;
			JimaTech::BroadingProcess::Passengers passengers(scene.GetNumberOfSeats(), &scene);

			JimaTech::BroadingProcess::SceneWritter sw(outputDir, &scene);
			JimaTech::BroadingProcess::Simulate simulate(&scene, &passengers);

			// int numberOfSeats = scene.GetNumberOfSeats();

			// give ticket to passengers
			for (int ticketSerialNumber = 0; ticketSerialNumber < passengers.GetNumberOfPassengers(); ticketSerialNumber++)
			{
				auto ticketInfo = scene.GetTicketinfo(ticketSerialNumber);
				passengers.GetPassengersList()[ticketSerialNumber].GivePassengerTicket(ticketInfo);
			}
			// put all passengers in randomly in the queue
			passengers.ShufflePassengers();

			int time = 0;

			while (!simulate.IsAllSeated())
			{
				sw.AddFrame(time++, passengers);
				simulate.MoveInAisle();
			}
			sw.AddFrame(time++, passengers);
			myfile << time << std::endl;
		}
		catch (std::exception& excep)
		{
			std::cout << "Error: " << excep.what() << std::endl;
		}
	}
	myfile.close();

}