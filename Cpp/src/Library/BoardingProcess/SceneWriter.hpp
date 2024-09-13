#pragma once

#include <opencv2\opencv.hpp>
#include <string>
#include <filesystem>

#include "Tools\DebugTools.h"
#include "Tools\StringTools.h"
#include "Tools\OpenCVTools.h"

#include "BoardingProcess\Passenger.hpp"
#include "BoardingProcess\TicketInfo.hpp"
#include "BoardingProcess\Scene.hpp"

namespace JimaTech::BroadingProcess
{
	class SceneWritter
	{
		std::filesystem::path m_outputPath;
		cv::VideoWriter m_outputVideo;
		JimaTech::BroadingProcess::Scene* m_scene;

		// seats
		int m_seatSize = 50;
		cv::Scalar m_seatColor = cv::Scalar(255, 0, 0);
		int m_seatPadding = 10;
		std::array<char, 20> SeatColumnChars;
		std::vector<std::vector<cv::Point>> m_centerOfSeats;
		int m_seatBlockSize = -1;
		int m_seatsInARow = -1;

		// passengers
		int PassengerSize = 20;
		cv::Scalar PassengerInAisleColor = cv::Scalar(102, 255, 0); 
		cv::Scalar PassengerInBagColor = cv::Scalar(102, 0, 204);
		cv::Scalar PassengerInSeatColor = cv::Scalar(204,0, 0);

		//Aisle data
		int m_aisleSize = 70;
		//int m_numberOfAisle = -1;
		std::vector<std::vector<cv::Point>> m_centerOfAisles;

		// seat and aisle
		int m_seatAndAisleBlockSize = -1;


		int m_thickness = 1;
		int m_imageHeight;
		int m_imageWidth;

		bool m_createVideo;
		bool m_createImages;
		bool m_draw_aisle;
	public:


		SceneWritter(const std::string& outputPath, JimaTech::BroadingProcess::Scene* scene, bool createVideo=true,bool createImages=false,bool draw_aisle=false) :
			m_outputPath(outputPath), m_scene(scene), m_createVideo(createVideo),m_createImages(createImages),m_draw_aisle(draw_aisle)
		{
			for (int i = 0; i < SeatColumnChars.size(); i++)
			{
				SeatColumnChars[i] = 'A' + i;
			}
			// ensure that output dir is exist
			std::filesystem::remove_all(m_outputPath);
			std::filesystem::create_directories(m_outputPath);
			
			InitializeVariables();

			std::string outputVideoName = (m_outputPath / "simulation.avi").string();
			if (m_createVideo)
			{
				m_outputVideo.open(outputVideoName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(m_imageWidth, m_imageHeight));
			}
		}
		~SceneWritter()
		{
			if (m_createVideo)
			{
				m_outputVideo.release();
			}
		}
		void AddFrame(int frameNo, Passengers passengers)
		{
			std::string imageName = (m_outputPath / ("Frame__" + JimaTech::Tools::to_string(frameNo, 4) + ".jpg")).string();
			cv::Mat  image(m_imageHeight, m_imageWidth, CV_8UC3, cv::Scalar(255, 255, 255));
			if (m_draw_aisle)
			{
				DrawAisles(image);
			}
			DrawSeats(image);
			DrawPassengers(image, passengers);
			if (m_createImages)
			{
				cv::imwrite(imageName, image);
			}

			if (m_createVideo)
			{
				m_outputVideo << image;
			}
		}

	private:
		void InitializeVariables()
		{
			m_seatBlockSize = (m_scene->GetNumberOfSeatInABlock() * (m_seatSize + m_seatPadding));
			m_seatAndAisleBlockSize = m_seatBlockSize + m_aisleSize;

			// find center of aisles
			m_centerOfAisles.resize(m_scene->GetNumberOfAisles());
			for (int i = 0; i < m_scene->GetNumberOfAisles(); i++)
			{
				m_centerOfAisles[i].resize(m_scene->GetNumberOfSeatRows());
				for (int j = 0; j < m_scene->GetNumberOfSeatRows(); j++)
				{
					m_centerOfAisles[i][j].x = i * (m_seatAndAisleBlockSize + m_seatPadding) + m_seatBlockSize + (m_aisleSize / 2) + m_seatPadding;
					m_centerOfAisles[i][j].y = (j * (m_seatSize + m_seatPadding)) + (m_seatSize / 2) + m_seatPadding;
				}
			}
			// find center of seats
			m_seatsInARow = m_scene->GetNumberOfSeatBlocks() * m_scene->GetNumberOfSeatInABlock();
			m_centerOfSeats.resize(m_seatsInARow);
			int seatCol = 0;
			for (int seatBlock = 0; seatBlock < m_scene->GetNumberOfSeatBlocks(); seatBlock++)
			{
				for (int seatInABlock = 0; seatInABlock < m_scene->GetNumberOfSeatInABlock(); seatInABlock++)
				{

					m_centerOfSeats[seatCol].resize(m_scene->GetNumberOfSeatRows());
					for (int j = 0; j < m_scene->GetNumberOfSeatRows(); j++)
					{
						m_centerOfSeats[seatCol][j].x = (seatBlock * (m_seatAndAisleBlockSize + m_seatPadding)) + (seatInABlock * (m_seatSize + m_seatPadding)) + (m_seatSize / 2) + m_seatPadding;
						m_centerOfSeats[seatCol][j].y = (j * (m_seatSize + m_seatPadding)) + (m_seatSize / 2) + m_seatPadding;

					}
					seatCol++;
				}
			}
			m_imageWidth = (m_seatBlockSize + m_seatPadding) * m_scene->GetNumberOfSeatBlocks() + m_scene->GetNumberOfAisles() * m_aisleSize;
			m_imageHeight = (m_seatSize + m_seatPadding) * m_scene->GetNumberOfSeatRows() + m_seatPadding;
		}

		void DrawAisles(cv::Mat& image)
		{
			// draw aisle rects
			for (auto& aislesRowdata : m_centerOfAisles)
			{
				for (auto& aislesCenterData : aislesRowdata)
				{
					JimaTech::Tools::DrawRectCentred(image, aislesCenterData, m_aisleSize, m_seatSize, cv::Scalar(100, 100, 10), m_thickness);
				}
			}
		}

		void DrawSeats(cv::Mat image)
		{
			for (auto& seatsRowdata : m_centerOfSeats)
			{
				for (auto& seatsCenterData : seatsRowdata)
				{

					JimaTech::Tools::DrawRectCentred(image, seatsCenterData, m_seatSize, m_seatSize, m_seatColor, m_thickness);
				}
			}
		}

		void DrawPassengers(cv::Mat image, Passengers passengers)
		{
			for (auto& p : passengers.GetPassengersList())
			{
				switch (p.GetPositionInfo()->GetPositionArea())
				{
				case PositionArea::IN_QUEUE:

					break;
				case PositionArea::IN_AISLE:
				case PositionArea::IN_BAGTO:
				{
					auto passengerColour= (p.GetPositionInfo()->GetPositionArea()== PositionArea::IN_AISLE) ? PassengerInAisleColor : PassengerInBagColor;
					auto asileInfo = p.GetPositionInfo()->GetAislePositionInfo();
					auto& center = m_centerOfAisles[asileInfo.m_aisleNo][asileInfo.m_inAislePosition];
					cv::circle(image, center, PassengerSize, passengerColour, m_thickness);
					JimaTech::Tools::PutTextCentered(image, p.GetTicketInfo()->GetTicketNo(), center, cv::FONT_HERSHEY_COMPLEX_SMALL, .6, passengerColour);
					break;
				}
				case PositionArea::IN_SEAT:
				{
					auto seatInfo = p.GetTicketInfo();
					auto& center = m_centerOfSeats[seatInfo->m_seatCol][seatInfo->m_seatRow];
					cv::circle(image, center, PassengerSize, PassengerInSeatColor, m_thickness);
					JimaTech::Tools::PutTextCentered(image, p.GetTicketInfo()->GetTicketNo(), center, cv::FONT_HERSHEY_COMPLEX_SMALL, .6, PassengerInSeatColor);
					break;
				}
				default:
					throw std::exception("Unknown area in draw passengers function:???? ",(int)(p.GetPositionInfo()->GetPositionArea()));
					break;
				}
			}
		}
	};
}