#include "GuidedBullet.h"

using namespace std;

GuidedBullet::GuidedBullet(int m_bm, int m_bp, int m_c, const string& m_ad) :Bullet(m_bm,m_bp,m_c,m_ad)
{
}


GuidedBullet::~GuidedBullet()
{
}

void GuidedBullet::move(){
	const double turning_speed_deg(1);
	if (target){
		double td_deg = GetTargetDirectionByDeg(); //ターゲットの方向
		double md_deg = atan2(vr, vh) / DX_PI * 180.0; //進行方向
		double speed = sqrt(vh*vh + vr*vr);
		double md_td_deg = td_deg - md_deg; //進行方向から見たターゲット方向の相対方向
		//ターゲット相対方向を-180~180の間にする
		while (md_td_deg < -180){ md_td_deg += 360; }
		if (md_td_deg > 360){ md_td_deg = fmod(md_td_deg, 360); }
		if (md_td_deg > 180){ md_td_deg = md_td_deg - 360; }

		double cng_dir = 0;
		cng_dir = md_td_deg > 0 ? turning_speed_deg : -turning_speed_deg;
		if (fabs(cng_dir) > fabs(md_td_deg)){ md_deg = td_deg; }

		md_deg += cng_dir;
		rotation = -md_deg;
		vh = speed * cos(md_deg / 180 * DX_PI);
		vr = speed * sin(md_deg / 180 * DX_PI);
	}
}
