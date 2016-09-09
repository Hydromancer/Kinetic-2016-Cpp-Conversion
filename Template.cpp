#include "WPILib.h"
#include <cmath>

// Tank Drive?

class Robot: public IterativeRobot
{
private:
  LiveWindow *lw = LiveWindow::GetInstance();
	
	Joystick *stick1 = new Joystick(0);
	Joystick *stick2 = new Joystick(1);
	Joystick *stick3 = new Joystick(2);
	
//Fake Land
/*
	TalonSRX *Fake_rearRight = new TalonSRX(1);
	TalonSRX *Fake_rearLeft = new TalonSRX(2);
	TalonSRX *Fake_frontLeft = new TalonSRX(3);
	TalonSRX *Fake_frontRight = new TalonSRX(4);
*/ 

	CANTalon *rearRight = new CANTalon(41);
	CANTalon *rearLeft = new CANTalon(42);
	CANTalon *frontLeft = new CANTalon(43);
	CANTalon *frontRight = new CANTalon(44);
	
	CANJaguar *sus1 = new CANJaguar(25);
	CANJaguar *sus2 = new CANJaguar(26);
	CANJaguar *sus3 = new CANJaguar(27);
	CANJaguar *sus4 = new CANJaguar(28);
	
	
	rearRight->SetInverted(true);
	rearLeft->SetInverted(true);
  	frontLeft->SetInverted(true);
	frontRight->SetInverted(true);
	
	RobotDrive *Kinetic = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
	
// Just because.	
	sus1.SetPositionMode(CANJaguar.kPotentiometer, -2000, -0, -0.0);
	sus2.SetPositionMode(CANJaguar.kPotentiometer, -2000, -0, -0.0);
	sus3.SetPositionMode(CANJaguar.kPotentiometer, 2000, 0, 0);
	sus4.SetPositionMode(CANJaguar.kPotentiometer, -2000, -0, -0);
	
	sus1.EnableControl();
	sus2.EnableControl();
	sus3.EnableControl();
	sus4.EnableControl();

	CANJaguar *shooter = new CANJaguar(30);
	CANJaguar *arm = new CANJaguar(31);
	Talon *intake = new Talon(0);

	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

	void RobotInit() 
	{
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		imaqError = IMAQdxOpenCamera("cam1", IMAQdxCameraControlModeController, &session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		}
		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
		}
		IMAQdxStartAcquisition(session);

	}

	void DisabledPeriodic() override
	{ 
//		Check; unnecessary?
//		(Scheduler::GetInstance())::run();
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);
  	}


//	Add own autonomous code.
	void AutonomousInit()
	{

	}

	void AutonomousPeriodic() 
	{
//		Check; unnecessary
//		(Scheduler::GetInstance())::run();
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);
		
//http://first.wpi.edu/FRC/roborio/stable/docs/cpp/classRobotDrive.html#a00f3c6f7906685fe2f98ceaa8bb7e188
//		Consider using RobotDrive->Drive(arg1, arg2);

	}

	void TeleopInit() 
	{

	}
	void TeleopPeriodic() 
	{
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);

//		TankDrive?
//		second arg not needed?
		robotDrive->ArcadeDrive(stick1, false);
	
	
//		Precalculated
//		based of potentiometer table
		sus1->Set(stick2->getY() * 0.060 + .497);
		sus2->Set(stick2->getY() * 0.060 + .349);
		sus3->Set(stick2->getY() * -0.060 + 0.625);
		sus4->Set(stick2->getY() * 0.060 + 0.278);

		shooter->Set(stick2->GetThrottle());
		arm->Set(stick3->getRawAxis(1));
		intake->Set(stick1->getRawAxis(2));

	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
