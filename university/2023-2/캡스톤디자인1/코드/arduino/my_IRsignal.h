/*
https://forum.arduino.cc/t/changing-pin-settings-of-an-ir-library/49772
-> 이와 같은 이유로 IR 출력핀을 3번으로 고정한다.
-> ESP-01의 Rx/Tx를 (2, 3)에서 (8, 9)로 변경한다.
*/





#ifndef MY_IRSIGNAL_H
#define MY_IRSIGNAL_H

#include <IRremote.h>




/* debugging
	app에서 누른 리모컨 버튼값.(1_push)
	각 bit-set으로 확인하며, 버튼값은 사용하고 clr한다.

*/ //debugging
// unsigned short button = 0x0;
#define btn_power 0
#define btn_tempUp 1
#define btn_tempDown 2
#define btn_mode 3
#define btn_fanSpeed 4
#define btn_swing 5
#define btn_turbo 6
#define btn_eco 7
#define btn_offTimer 8








/*
	에어컨/리모컨 상태값.
*/
typedef struct tag_IRstatus
{
	int temp;		// 18 ~ 30
	int offTimer;		// 0_off    1_0.5h    2_1h    3_2h    4_3h    5_4h    6_5h    7_6h    8_7h
	bool power;		// 0_off    1_on
	bool mode;    // 0_cooling    1_dehumidifying
	bool fanSpeed;		// 0_light    1_strong
	bool fanSpeed_dehumidifying;
	bool swing;   // 0_off    1_on
	bool swing_turbo;   // 0_off    1_on
	bool swingFlag;   // 0_off    1_on    // turboMode에서 swing/swing_turbo 관계 해결.
	bool turbo;		// 0_off    1_on
	bool eco;		// 0_off    1_on
}IRstatus;











































/*
	return (unsigned long)rawData;
		-> IRremote.h> IrSender.sendLGRaw(rawData, 0, false);

	현재 상태값과 버튼값을 조합하여 IRrawData를 생성하고 반환한다.
		Wi-Fi 통신으로 에어컨보드에 보낼 IRrawData값이다.
		protocol-LG의 경우 rawData는 28-bits이므로, 2-bytes 선언 후 7-nibbles만 사용한다.

	// 최대한 buttonPush와 동시에 rawData를 생성해야 한다.
	// 그러고도 발생할 버튼값 중복 문제는, 여기서 해결하지 말고 버튼값이 정의되어있는 곳에서 delay와 commandTable로 해결한다.
*/
unsigned long createIRrawData(IRstatus* _pStatus, unsigned short* _pButton)
{
	unsigned long rawData;



	if (_pStatus->power == true)
	{
		if (*_pButton & (1 << btn_tempUp))		// button check
		{
			*_pButton &= ~(1 << btn_tempUp);		// button clr

			if (_pStatus->mode == true)
			{
				// nothing
			}
			else if (_pStatus->turbo == true)
			{
				_pStatus->turbo = false;
				if (_pStatus->swingFlag == true)
				{
					_pStatus->swing = true;
					_pStatus->swingFlag = false;
				}
				rawData = 0x880834F;
			}
			else
			{
				_pStatus->temp = (_pStatus->temp >= 30) ? 30 : ++_pStatus->temp;

				if (_pStatus->fanSpeed == true)
				{
					// 834F(18도) ~ 8F4B(30도)
					rawData = 0x8808040;
					rawData |= (((_pStatus->temp - 3) % 16) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
				else
				{
					// 830B(18도) ~ 8F07(18도)
					rawData = 0x8808000;
					rawData |= (((_pStatus->temp - 7) % 16) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
			}
		}



		if (*_pButton & (1 << btn_tempDown))		// button check
		{
			*_pButton &= ~(1 << btn_tempDown);		// button clr

			if (_pStatus->mode == true)
			{
				// nothing
			}
			else if (_pStatus->turbo == true)
			{
				_pStatus->turbo = false;
				if (_pStatus->swingFlag == true)
				{
					_pStatus->swing = true;
					_pStatus->swingFlag = false;
				}
				rawData = 0x880834F;
			}
			else
			{
				_pStatus->temp = (_pStatus->temp <= 18) ? 18 : --_pStatus->temp;

				if (_pStatus->fanSpeed == true)
				{
					// 834F(18도) ~ 8F4B(30도)
					rawData = 0x8808040;
					rawData |= (((_pStatus->temp - 3) % 16) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
				else
				{
					// 830B(18도) ~ 8F07(18도)
					rawData = 0x8808000;
					rawData |= (((_pStatus->temp - 7) % 16) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
			}
		}



		if (*_pButton & (1 << btn_mode))		// button check
		{
			*_pButton &= ~(1 << btn_mode);		// button clr

			if (_pStatus->turbo == true)
			{
				_pStatus->turbo = false;
				if (_pStatus->swingFlag == true)
				{
					_pStatus->swing = true;
					_pStatus->swingFlag = false;
				}

				rawData = 0x880834F;
			}
			else
			{
				_pStatus->mode ^= 1;

				if (_pStatus->mode == true)
				{
					_pStatus->eco = false;

					if (_pStatus->fanSpeed_dehumidifying == true)
					{
						rawData = 0x8809845;
					}
					else
					{
						rawData = 0x8809801;
					}
				}
				else
				{
					if (_pStatus->fanSpeed == true)
					{
						// 834F(18도) ~ 8F4B(30도)
						rawData = 0x8808040;
						rawData |= (((_pStatus->temp - 3) % 16) << 0);
						rawData |= ((_pStatus->temp - 15) << 8);
					}
					else
					{
						// 830B(18도) ~ 8F07(18도)
						rawData = 0x8808000;
						rawData |= (((_pStatus->temp - 7) % 16) << 0);
						rawData |= ((_pStatus->temp - 15) << 8);
					}
				}
			}
		}



		if (*_pButton & (1 << btn_fanSpeed))		// button check
		{
			*_pButton &= ~(1 << btn_fanSpeed);		// button clr

			if (_pStatus->turbo == true)
			{
				_pStatus->turbo = false;

				if (_pStatus->swingFlag == true)
				{
					_pStatus->swing = true;
					_pStatus->swingFlag = false;
				}

				rawData = 0x880834F;
			}
			else
			{
				if (_pStatus->mode == true)
				{
					_pStatus->fanSpeed_dehumidifying ^= 1;

					if (_pStatus->fanSpeed_dehumidifying == true)
					{
						rawData = 0x8809845;
					}
					else
					{
						rawData = 0x8809801;
					}
				}
				else
				{
					_pStatus->fanSpeed ^= 1;

					if (_pStatus->fanSpeed == true)
					{
						// 834F(18도) ~ 8F4B(30도)
						rawData = 0x8808040;
						rawData |= (((_pStatus->temp - 3) % 16) << 0);
						rawData |= ((_pStatus->temp - 15) << 8);
					}
					else
					{
						// 830B(18도) ~ 8F07(18도)
						rawData = 0x8808000;
						rawData |= (((_pStatus->temp - 7) % 16) << 0);
						rawData |= ((_pStatus->temp - 15) << 8);
					}
				}
			}
		}



		if (*_pButton & (1 << btn_swing))		// button check
		{
			*_pButton &= ~(1 << btn_swing);		// button clr

			//터보상태에서 상하바람버튼을 누를 때,
			//	일반상하고정이 상하라면->터보전용상하고정만 바뀜.
			//	일반상하고정이 고정이라면->터보전용상하고정과 일반상하고정이 같이 바뀜.

			// swingFlag를 여기서 세우고,
			// 전원버튼 포함한 다른버튼 누르면서 터보모드가 off될 때, swingFlag를 내리면서 적용하자.
			// 다른 버튼에 전부 코드가 추가될건데, ㄱㅊ.

			if (_pStatus->turbo == true)
			{
				_pStatus->swing_turbo ^= 1;

				if (_pStatus->swing == true)
				{
					if (_pStatus->swing_turbo == true)
					{
						rawData = 0x8813149;
					}
					else
					{
						rawData = 0x881315A;
					}
				}
				else
				{
					if (_pStatus->swing_turbo == true)
					{
						_pStatus->swingFlag = true;

						rawData = 0x8813149;
					}
					else
					{
						_pStatus->swingFlag = false;

						rawData = 0x881315A;
					}
				}
			}
			else
			{
				_pStatus->swing ^= 1;

				if (_pStatus->swing == true)
				{
					rawData = 0x8813149;
				}
				else
				{
					rawData = 0x881315A;
				}
			}
		}



		if (*_pButton & (1 << btn_turbo))		// button check
		{
			*_pButton &= ~(1 << btn_turbo);		// button clr
			_pStatus->turbo ^= 1;

			// 터보는 어떠한 상태에서도 누를 수가 있으니까, 일단 상태 반전시켜놔.

			// 터보 == 1		->		제습off 절전off 18도 강풍 터보전용상하고정->고정

			if (_pStatus->turbo == true)
			{
				_pStatus->mode = false;
				_pStatus->eco = false;
				_pStatus->temp = 18;
				_pStatus->fanSpeed = true;
				_pStatus->swing_turbo = false;
				rawData = 0x8810089;
			}
			else
			{
				if (_pStatus->swingFlag == true)
				{
					_pStatus->swing = true;
					_pStatus->swingFlag = false;
				}
				rawData = 0x880834F;
			}
		}



		if (*_pButton & (1 << btn_eco))		// button check
		{
			*_pButton &= ~(1 << btn_eco);		// button clr

			// 절전에서 안 눌림.
			// 이외에는 어디서든 눌림.
			// 눌리는 즉시 온도18~21도->22도로 변경 + 절전on
			// 터보에서 절전누르면 터보 꺼져야 함. 그리고 절전은 켜져.

			if (_pStatus->mode == true)
			{
				// nothing
			}
			else
			{
				_pStatus->eco ^= 1;

				if (_pStatus->eco == true)
				{
					_pStatus->turbo = false;
					if (_pStatus->swingFlag == true)
					{
						_pStatus->swing = true;
						_pStatus->swingFlag = false;
					}

					_pStatus->temp = (_pStatus->temp <= 21) ? 22 : _pStatus->temp;

					rawData = 0x8810045;
				}
				else
				{
					rawData = 0x8810056;
				}
			}
		}



		if (*_pButton & (1 << btn_offTimer))		// button check
		{
			*_pButton &= ~(1 << btn_offTimer);		// button clr
			_pStatus->offTimer = (_pStatus->offTimer >= 8) ? 0 : ++_pStatus->offTimer;

			switch (_pStatus->offTimer)
			{
			case 0:
				rawData = 0x88A000A;
				break;
			case 1:
				rawData = 0x88A01E9;
				break;
			case 2:
				rawData = 0x88A03C9;
				break;
			case 3:
				rawData = 0x88A0789;
				break;
			case 4:
				rawData = 0x88A0B49;
				break;
			case 5:
				rawData = 0x88A0F09;
				break;
			case 6:
				rawData = 0x88A12C9;
				break;
			case 7:
				rawData = 0x88A1689;
				break;
			case 8:
				rawData = 0x88A1A49;
				break;
			}
		}
	}



	if (*_pButton & (1 << btn_power))		// button check
	{
		*_pButton &= ~(1 << btn_power);		// button clr
		_pStatus->power ^= 1;		// complement

		// 버튼을 on과 off로 나눠서 rawData 결정하자.

		// 전원 on
		if (_pStatus->power == true)
		{
			// 제습
			if (_pStatus->mode == true)
			{
				// 강풍
				if (_pStatus->fanSpeed == true)
				{
					rawData = 0x880184D;
				}
				// 약풍
				else
				{
					rawData = 0x8801809;
				}
			}
			// 냉방
			else
			{
				// 강풍
				if (_pStatus->fanSpeed == true)
				{
					// 0347(18도) ~ 0F43(30도)
					rawData = 0x8800040;
					rawData |= (((_pStatus->temp - 11) % 16) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
				// 약풍
				else
				{
					// 0303(18도) ~ 0F0F(30도)
					rawData = 0x8800000;
					rawData |= ((_pStatus->temp - 15) << 0);
					rawData |= ((_pStatus->temp - 15) << 8);
				}
			}
		}
		// 전원 off
		else
		{
			_pStatus->turbo = false;
			_pStatus->eco = false;
			_pStatus->offTimer = 0;

			if (_pStatus->swingFlag == true)
			{
				_pStatus->swing = true;
				_pStatus->swingFlag = false;
			}

			rawData = 0x088C0051;
		}
	}



	return rawData;
}





































/*
  _status       : 에어컨의 현재 상태
  _button       : (특정 버튼이 활성화된) 버튼 배열
  _commandRecv  : APP에서 온 명령이며, 버튼값에 대응한다.
*/
void IRsend(IRstatus& _status, unsigned short& _button, char _commandRecv)
{
  _button |= (1 << (int)(_commandRecv - '0'));        // commandRecv를 unsigned short button의 idx로 매칭한다.
  // Serial.println(rawData, HEX);       // dbg
  IrSender.sendLGRaw(createIRrawData(&_status, &_button), 0, false);
}






#endif    // my_IRsignal_h