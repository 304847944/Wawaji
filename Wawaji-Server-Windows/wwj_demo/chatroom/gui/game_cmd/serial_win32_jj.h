/*�޾�����*/
#pragma once
#include<mutex>
#include "serial_win32.h"
#include"lws_config.h"
#include "WebSocket.h"

using namespace network;
namespace nim_wwj
{
#define WS_RX_BUFFER_SIZE (65536)
	enum class GameState
	{
		UnInit,
		Init,  /** &lt; value 0 */
		Charge,
		Coin,
		Play,        /** &lt; value 1 */
		GameOver,     /** &lt; value 2 */
	};


	struct RoomInfo
	{
		int id_;
		int price_;
		int game_time_;
		int auto_start_;
		string status_;
	};

	class WwjControl_jj :public WwjControl,public virtual nbase::SupportWeakCallback,WebSocket::Delegate
	{
	public:
		WwjControl_jj();
		~WwjControl_jj();
		virtual bool SetSerialParam(serial_param_t param);
		virtual bool OpenSerial(const char* com);
		virtual void CloseSerial();
		virtual bool CrownBlockReset();//�쳵��λ
		virtual bool SerialDirectectOpt(serial_direction_opt_type_e type);//���޻��������
		virtual void SerialSetStepSize(unsigned char step_size);//���޻�����
		virtual void  Pay(int coins);//���Ϸ֣��Ϸ�֮ǰӦ�ȵ���CheckNormal�ӿڼ���豸״̬ ����ɻص�����
		virtual bool QueryDeviceInfo();//��ѯ�ն���Ŀ�����ڲ�ѯ�豸����Ŀ״��������״̬�� ����ɻص�����
		virtual bool CheckNormal();//��ѯ����, �û���ѯ�Ƿ�����,����ɻص�����
		wwj_set_param_t GetSettingParam();//��ȡ���޻�����
		bool  SetSettingParam(wwj_set_param_t param);//�������޻��豸����
		virtual void StartSerialReadThread();
		virtual void SetOptFuncCb(wwj_callback_func cb) { wwj_opt_cb_ = cb; }
	private:
		virtual void onOpen(WebSocket* ws);
		virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
		virtual void onClose(WebSocket* ws);
		virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
	private:
		unsigned char send_data_[BUF_LEN];
		string ca_path_;
		wwj_set_param_t wwj_set_param_;
		int wwj_step_size_;
		int opt_count_;
		string identifying_;
		wwj_callback_func wwj_opt_cb_;
		string wss_url_;
		std::list<RoomInfo> room_info_;
	private:
		friend class WebSocketCallbackWrapper;
		WebSocket* web_socket_;
		GameState game_state_;
		int extra_id_;
		std::mutex   game_state_mutex_;
	};
}