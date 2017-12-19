-------システム関数---------

--------コルーチン管理------
require("coroutinemanager")
CrtnMgr = coroutinemanager.CoroutineManager.new()

--情報出力
function Dump()
    GameSystem.PrintToConsole("---EnemyMoverDump---\n")
    CrtnMgr:Dump()
end

--ムーバ作成
function CreateMover(mvr_name,enemy)
	local f = _G[mvr_name]
	if f==nil then
		return -1
	elseif type(f) ~= "function" then
		return -2
	else
		return CrtnMgr:CreateCoroutine(f,enemy)
	end
end

--指定IDのムーバ実行
function ExcuteMover(id)
	return CrtnMgr:ExcuteCoroutine(id)
end

--指定IDのムーバ削除
function DeleteMover(id)
	return CrtnMgr:InvalidateCoroutine(id)
end

--ムーバー数取得
function GetMoverCount()
	return CrtnMgr:GetCount()
end

--サウンド拡張
GameSound.PlaySoundByID = function(id)
	local sh = GameSound.CreateSoundPlayHandle(id)
	GameSound.PlaySoundByHandle(sh,false)
end


--敵クラス拡張
Enemy.FireBulletToTarget = function(self,kind,vel,power)
	local td = self:GetTargetDirectionByDeg()
	local bh = self.vh
	local br = self.vr
	self.vh=0
	self.vr=0
	self:FireBullet(kind,td,vel,power)
	self.vh=bh
	self.vr=br
end
--ばつだま発射(方向、速さ、威力)
Enemy.FireBatsuBullet = function(self,dir,vel,power)
    self:FireRotatingBullet("BulletBatsu",dir,vel,power,10)
	GameSound.PlaySoundByID("EnemyShoot3")
end
--とげたま発射(速さ、威力)
Enemy.FireTogeBullet = function(self,vel,power)
	self:FireBulletToTarget("BulletToge",vel,power)
	GameSound.PlaySoundByID("EnemyShoot1")
end
--ひしだま発射(方向、速さ、威力)
Enemy.FireRhombusBullet = function(self,dir,vel,power)
	self:FireBullet("BulletRhombus",dir,vel,power)
	GameSound.PlaySoundByID("EnemyShoot2")
end

--初期化
function InitEneMvrMgr()
    CrtnMgr:SetDumpFunction(GameSystem.PrintToConsole)
	CrtnMgr:Init()
end

----------------------------
----------------------------



--乱数初期化
math.randomseed(os.time())
------utilites---------
utls={}
--関数中断
utls["yield"] = function()
	CrtnMgr:Yield()
end
--指定フレーム待機(待機フレーム数)
utls["Wait"] = function(f)
	for i=1,f do
		CrtnMgr:Yield()
	end
end
--重力を加えつつ指定フレーム待機(Enemyクラス、待機フレーム数)
utls["WaitWithGravity"] = function(e,f)
	for i=1,f do
		e:AddGravity()
		CrtnMgr:Yield()
	end
end
--値が指定範囲内にあるか確認(値,下限,上限)
utls["InRange"] = function(value,min,max)
	if value < min or max < value then return false
	else return true
	end
end

--指定角のに向かって指定角度移動する(変更する角度変数,目的の角度,移動角度)(不具合有)
utls["MoveToT"] = function(e_t,pur_t,vt)
	e_t = e_t + 10
	if ((pur_t - e_t + 360) % 360) < 180 then
		e_t = e_t + vt
	else
		e_t = e_t - vt
	end
end

-----EnemyMover(終了時は何も返さなくて良い。中断にはCrtnMgr:Yield()を使う)------
--引数はEnemyクラスポインタ
--戻り値はbool。trueでムーバ終了

function RedEnemy(e)
	local height = 200
	local hover_frame = 600
	local p_radius = e:GetBelongingPlanetRadius()
	e.vh=0
	e.vr=0
	e:StartHermiteMove(Vector2D(math.random(360),p_radius + height),Vector2D(0,0),Vector2D(0,0),60)
	while(e:IsAutoMoving())do
		CrtnMgr:Yield()
	end
	for i=1,hover_frame do
		--プレイヤーを狙う
		if i%15==0 then
			e:FireTogeBullet(7,5.0)
		end
		CrtnMgr:Yield()
	end
	while(e.ground==false)do
		e:AddGravity()
		CrtnMgr:Yield()
	end
	--上陸後
	e:Landed();
	while(true)do
		e:Friction()
		e:AddGravity()
		CrtnMgr:Yield()
	end
end

function GreenEnemy(e)
	if e.vh<0 then
		e:ChangeDirection(Enemy.LEFT)
		e.rota_vel = -5
	else
		e.rota_vel = 5
	end
	local cnt=0
	while(e.ground==false)do
		if cnt%60==0 then e:FireBatsuBullet(270,3,3.0) end
		cnt = cnt+1
		CrtnMgr:Yield()
	end
	--上陸後
	e:Landed();
	e.rota_vel=0
	e.rotation=0
	while(true)do
		e:Friction()
		e:AddGravity()
		CrtnMgr:Yield()
	end
end

function PinkEnemy(e)
	local height = 250
	local hover_frame = 600
	local p_radius = e:GetBelongingPlanetRadius()
	local move_dis_v = 20
	local move_dis_h = 100
	local move_frame=20
	e.vh=0
	e.vr=0
	local cntr=0
	while(e.r>p_radius + height)do
		if cntr%3==0 then e:FireRhombusBullet(270,2,5.0) end
		e:StartHermiteMove(Vector2D(e.t +( math.random(move_dis_h*2)-move_dis_h)/(e.r/180*math.pi),e.r-move_dis_v),Vector2D(0,0),Vector2D(0,0),move_frame)
		while(e:IsAutoMoving())do
			CrtnMgr:Yield()
		end
		cntr = cntr+1
	end
	e:StartHermiteMove(Vector2D((e:GetTargetT()+180)%360,p_radius + height),Vector2D(0,0),Vector2D(0,0),60)
	while(e:IsAutoMoving())do
		CrtnMgr:Yield()
	end
	for i=1,hover_frame do
		--プレイヤーを狙う
		if (i-1)%45==0 then
			local td = e:GetTargetDirectionByDeg()
			e:FireRhombusBullet(0,2,5.0)
			e:FireRhombusBullet(180,2,5.0)
		end
		CrtnMgr:Yield()
	end
	while(e.ground==false)do
		e:AddGravity()
		CrtnMgr:Yield()
	end
	--上陸後
	e:Landed();
	while(true)do
		e:Friction()
		e:AddGravity()
		CrtnMgr:Yield()
	end
end

function WhiteEnemy(e)
	local height = 200
	local p_radius = e:GetBelongingPlanetRadius()
	if e.vh<0 then
		e:ChangeDirection(Enemy.LEFT)
		e.rota_vel = -5
	else
		e.rota_vel = 5
	end
	local cnt=0
	while(e.r > p_radius + height)do
		if cnt%30==0 then
			if cnt%90 == 0 then
				e:FireRhombusBullet(270,1,5.0)
			else
				e:FireTogeBullet(7,5.0)
			end
		end
		cnt = cnt+1
		CrtnMgr:Yield()
	end
	e.vh=0
	e.vr=0
	for i=1,300 do
		if i%15==0 then
			e:FireBatsuBullet(270,3,3.0)
			e:FireBatsuBullet(240,3,3.0)
			e:FireBatsuBullet(255,3,3.0)
			e:FireBatsuBullet(300,3,3.0)
			e:FireBatsuBullet(285,3,3.0)
		end
		CrtnMgr:Yield()
	end
	--レーザー
	e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),30)
	while(e:IsAutoMoving())do
		CrtnMgr:Yield()
	end
	--レーザー発射
	local l_id = e:FireLaser(30,10,60,255,255,255,90)
	--レーザー待機
	while(e:GetObject(l_id)~=nil)do
		CrtnMgr:Yield()
	end
	--落下
	while(e.ground==false)do
		e:AddGravity()
		CrtnMgr:Yield()
	end
	--上陸後ターゲットの方へ転がる
	e:Landed();
	if e:GetTargetTFromMe()<0 then
		e.vh = -10
	else
		e.vh = 10
	end
	e.rota_vel = e.vh/32/math.pi*180
	while(true)do
		e:AddGravity();
		e:AddForceToPlanet(e.t,200)
		CrtnMgr:Yield()
	end
end

function SuperGreen(e)
	local height = 200
	e.vr=0
	e.vh=0
	local rest_frame = 60
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame=120
	
	local state=0
	
	while(true)do
		if state==0 then
			for i=1,rest_frame do
				CrtnMgr:Yield()
			end
			e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),move_frame)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			state = math.random(1,5)
--			state = 5
		elseif state==1 then
			for i=1,300 do
				if i%5==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
				end
				e.rota_vel = e.rota_vel+0.1
				CrtnMgr:Yield()
			end
			local i=0
			while(e.rota_vel>0)do
				if i%5==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
				end
				e.rota_vel = e.rota_vel - 0.5
				i=i+1
				CrtnMgr:Yield()
			end
			e.rota_vel = 0
			state=0
		elseif state==2 then
			e:StartHermiteMove(Vector2D(e.t+360,p_radius + height),Vector2D(0,0),Vector2D(0,0),60)
			local cnt=0
			while(e:IsAutoMoving())do
				if cnt%5==0 then
					e:FireBatsuBullet(270,5,3.0)
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			state=0
		elseif state==3 then
			local cnt=0
			e.rota_vel=10
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%3==0 then
						e:FireBatsuBullet(-e.rotation,3,3.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%3==0 then
						e:FireBatsuBullet(-e.rotation,3,3.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
			end
			e.rota_vel=0
			e.rotation=0
			state=0
		elseif state==4 then
			local shoot_num=3
			for i=1,shoot_num do
				utls.Wait(30)
				e:FireBatsuBullet(230+(i-(shoot_num+1)/2)*10,5,3.0)
				e:FireBatsuBullet(250+(i-(shoot_num+1)/2)*10,5,3.0)
				e:FireBatsuBullet(270+(i-(shoot_num+1)/2)*10,5,3.0)
				e:FireBatsuBullet(290+(i-(shoot_num+1)/2)*10,5,3.0)
				e:FireBatsuBullet(310+(i-(shoot_num+1)/2)*10,5,3.0)
			end
			state=0
		elseif state==5 then
			local shoot_num=7
			for i=1,shoot_num do
				utls.Wait(8)
				e:FireBatsuBullet(270+(i-(shoot_num+1)/2)*5,5,3.0)
			end
			state=0
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function SuperSkyblue(e)
	local height = 200
	e.vr=0
	e.vh=0
	local rest_frame = 50
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame=100
	
	local state=0
	
	while(true)do
		if state==0 then
			for i=1,rest_frame do
				CrtnMgr:Yield()
			end
			e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),move_frame)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			state = math.random(1,6)
--			state = 6
		elseif state==1 then
			--レーザー発射
			local l_id = e:FireLaser(32,30,150,0,255,255,90)
			--レーザー待機
			while(e:GetObject(l_id) ~= nil)do
				CrtnMgr:Yield()
			end
			state=0
		elseif state==2 then
			--レーザー発射
			local l_id = e:FireLaser(24,30,180,0,255,255,90)
			local laser = e:GetObject(l_id)
			local ttfm = e:GetTargetTFromMe()
			if ttfm > 0 then
				e.vh=2
				laser.vh=2
			else
				e.vh=-2
				laser.vh=-2
			end
			--レーザー待機
			while(e:GetObject(l_id) ~= nil)do
				CrtnMgr:Yield()
			end
			e.vh=0
			state=0
		elseif state==3 then
			local laser_num = 2
			local lasers_id = {}
			for i=1,laser_num do
				lasers_id[i] = e:FireLaser(24,30,10000,0,255,255,360/laser_num*(i-1)+90)
			end
			for i=1,200 do
				e.rota_vel = e.rota_vel+0.1
				for i=1,laser_num do
					local laser = e:GetObject(lasers_id[i])
					laser.rota_vel = e.rota_vel
				end
				CrtnMgr:Yield()
			end
			while(e.rota_vel>2)do
				e.rota_vel = e.rota_vel - 0.5
				for i=1,laser_num do
					local laser = e:GetObject(lasers_id[i])
					laser.rota_vel = e.rota_vel
				end
				CrtnMgr:Yield()
			end
			while(1 <= e.rotation and e.rotation <= 359)do
				CrtnMgr:Yield()
			end
			for i=1,laser_num do
				local laser = e:GetObject(lasers_id[i])
				laser.rota_vel=0
				e:EndObject(lasers_id[i])
			end
			e.rota_vel = 0
			e.rotation=0
			state=0
		elseif state==4 then
			local ll_id = e:FireLaser(24,30,10000,0,255,255,135)
			local lr_id = e:FireLaser(24,30,10000,0,255,255,45)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			local ll = e:GetObject(ll_id)
			local lr = e:GetObject(lr_id)
			ll.rota_vel=-1
			lr.rota_vel=1
			while(ll.rotation>90)do
				CrtnMgr:Yield()
			end
			ll.rota_vel=0
			lr.rota_vel=0
			ll.rotation=90
			lr.rotation=90
			for i=1,60 do
				CrtnMgr:Yield()
			end
			e:EndObject(ll_id)
			e:EndObject(lr_id)
			state=0
		elseif state==5 then
			e.rota_vel=10
			local laser_num = 2
			local lasers_id = {}
			for i=1,laser_num do
				lasers_id[i] = e:FireLaser(24,30,10000,0,255,255,360/laser_num*(i-1)+90)
				local laser = e:GetObject(lasers_id[i])
				laser.rota_vel=e.rota_vel
			end
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
				for i=1,laser_num do
					local laser = e:GetObject(lasers_id[i])
					laser.t = e.t
					laser.r=e.r
				end
					CrtnMgr:Yield()
				end
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					for i=1,laser_num do
						local laser = e:GetObject(lasers_id[i])
						laser.t = e.t
						laser.r=e.r
					end
					CrtnMgr:Yield()
				end
			end
			for i=1,laser_num do
				local laser = e:GetObject(lasers_id[i])
				laser.rota_vel=0
--				laser.rotation = 360/laser_num*(i-1)+90
				e:EndObject(lasers_id[i])
			end
			e.rota_vel=0
			e.rotation=0
			state=0
		elseif state==6 then
			for i=1,300 do
				if i%2==0 then
					e:FireBatsuBullet(-e.rotation,7,3.0)
				end
				e.rota_vel = e.rota_vel+0.1
				CrtnMgr:Yield()
			end
			local i=0
			while(e.rota_vel>0)do
				if i%2==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
				end
				e.rota_vel = e.rota_vel - 0.5
				i=i+1
				CrtnMgr:Yield()
			end
			e.rota_vel = 0
			state=0
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function SuperRed(e)
	local height = 200
	e.vr=0
	e.vh=0
	local rest_frame = 40
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame=80
	
	local state=0
	
	while(true)do
		if state==0 then
			for i=1,rest_frame do
				CrtnMgr:Yield()
			end
			e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),move_frame)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			state = math.random(1,5)
--			state = 5
		elseif state==1 then
			for i=1,300 do
				if i%3==0 then
					e:FireBatsuBullet(-e.rotation,7,3.0)
					e:FireBatsuBullet(-e.rotation+180,7,3.0)
				end
				e.rota_vel = e.rota_vel+0.1
				CrtnMgr:Yield()
			end
			local i=0
			while(e.rota_vel>0)do
				if i%3==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
					e:FireBatsuBullet(-e.rotation+180,5,3.0)
				end
				e.rota_vel = e.rota_vel - 0.5
				i=i+1
				CrtnMgr:Yield()
			end
			e.rota_vel = 0
			state=0
		elseif state==2 then
			local shoot_num=10
			for i=1,shoot_num do
				utls.Wait(5)
				e:FireTogeBullet(7,5.0)
			end
			state=0
		elseif state==3 then
			e:StartHermiteMove(Vector2D(e.t+360,p_radius + height),Vector2D(0,0),Vector2D(0,0),60)
			local cnt=0
			while(e:IsAutoMoving())do
				if cnt%3==0 then
					e:FireTogeBullet(7,5.0)
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			state=0
		elseif state==4 then
			local shoot_num=10
			local ll_id = e:FireLaser(32,30,10000,255,0,0,120)
			local lr_id = e:FireLaser(32,30,10000,255,0,0,60)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			for i=1,shoot_num do
				utls.Wait(10)
				e:FireTogeBullet(7,5.0)
			end	
			e:EndObject(ll_id)
			e:EndObject(lr_id)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			state=0
		elseif state==5 then
			local cnt=0
			e.rota_vel=10
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%5==0 then
						e:FireTogeBullet(7,5.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%5==0 then
						e:FireTogeBullet(7,5.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
			end
			e.rota_vel=0
			e.rotation=0
			state=0
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function SuperPurple(e)
	local height = 250
	e.vr=0
	e.vh=0
	local rest_frame = 30
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame=60
	
	local state=0
	
	--最初は転がってくる
	if math.random(1,2)<=1 then --時計
		e.vh=10
	else --反時計
		e.vh=-10
	end
	e.rota_vel = e.vh/32/math.pi*180
	e.vr=-2
	while(e.ground==false)do
		CrtnMgr:Yield()
	end
	for i=1,120 do
		CrtnMgr:Yield()
	end
	state=2
	while(true)do
		if state==0 then --ジャンプ
			--方向決定（ランダム）
			if math.random(1,2)<=1 then --時計
				e.vh=10
			else --反時計
				e.vh=-10
			end
			e.rota_vel = e.vh/32/math.pi*180
			while(e.ground==false)do
				e:AddGravity()
				CrtnMgr:Yield()
			end
			e.vr = 10
			
			while(e.vr>0)do
				e:AddGravity()
				CrtnMgr:Yield()
			end
			e.vr=0
			state = math.random(3,5)
--			state=5
		elseif state==1 then --着地
			--方向決定（ランダム）
			if math.abs(e.vh)~=10 then
				local dir
				if e.vh~=0 then
					dir = e.vh / math.abs(e.vh)
				 else
					if math.random(1,2)<=1 then --時計
						dir=1
					else --反時計
						dir=-1
					end
				end
				e.vh = 10*dir
				e.rota_vel = e.vh/32/math.pi*180
			end
			while(e.ground==false)do
				e:AddGravity()
				CrtnMgr:Yield()
			end
			state=2
		elseif state==2 then --走る
			for i=1,120 do
				e:AddGravity()
				CrtnMgr:Yield()
			end
			state=0
		elseif state==3 then --弾回転連写
--			e.vh=0
			for i=1,300 do
				if i%4==0 then
					e:FireBatsuBullet(-e.rotation,7,3.0)
					e:FireBatsuBullet(-e.rotation+120,7,3.0)
					e:FireBatsuBullet(-e.rotation+240,7,3.0)
				end
				CrtnMgr:Yield()
			end
			state=1
		elseif state==4 then --追いかけ
			for i=1,300 do
				local p_dir
				if e:GetTargetTFromMe()<0 then 
					p_dir=-1
				else 
					p_dir=1
				end
				e.rota_vel = 10*p_dir/32/math.pi*180
				e.vh = e.vh + 0.3*p_dir
				if math.abs(e.vh)>10 then
					e.vh = 10 * (e.vh / math.abs(e.vh))
				end
--				e.vh = e.vh % 10
				e:AddGravity()
				CrtnMgr:Yield()
			end
			state=1
		elseif state==5 then --ジグザグ
			local cnt=0
			local vh_buf = e.vh
			local dir = e.vh / math.abs(e.vh)
			e.vh=0
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12*dir,p_radius+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					CrtnMgr:Yield()
				end
				e:FireTogeBullet(7,5.0)
				e:StartHermiteMove(Vector2D(e.t+360/12*dir,p_radius),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					CrtnMgr:Yield()
				end
			end
			e.vh=vh_buf
			state=1
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function SuperPink(e)
	local height = 200
	e.vr=0
	e.vh=0
	local rest_frame = 30
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame=60
	
	local state=0
	
	while(true)do
		if state==0 then
			for i=1,rest_frame do
				CrtnMgr:Yield()
			end
			e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),move_frame)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			state = math.random(1,5)
--			state = 5
		elseif state==1 then
			for i=1,300 do
				if i%4==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
					e:FireBatsuBullet(-e.rotation+90,5,3.0)
					e:FireBatsuBullet(-e.rotation+180,5,3.0)
					e:FireBatsuBullet(-e.rotation+270,5,3.0)
				end
				e.rota_vel = e.rota_vel+0.1
				CrtnMgr:Yield()
			end
			local i=0
			while(e.rota_vel>0)do
				if i%4==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
					e:FireBatsuBullet(-e.rotation+90,5,3.0)
					e:FireBatsuBullet(-e.rotation+180,5,3.0)
					e:FireBatsuBullet(-e.rotation+270,5,3.0)
				end
				e.rota_vel = e.rota_vel - 0.5
				i=i+1
				CrtnMgr:Yield()
			end
			e.rota_vel = 0
			state=0
		elseif state==2 then
			local shoot_num=8
			utls.Wait(5)
			for i=1,shoot_num do
				e:FireRhombusBullet(i*(360/shoot_num),2,5.0)
			end
			state=0
		elseif state==3 then
			e:StartHermiteMove(Vector2D(e.t+360,p_radius + height),Vector2D(0,0),Vector2D(0,0),60)
			local cnt=0
			while(e:IsAutoMoving())do
				if cnt%3==0 then
					local td = e:GetTargetDirectionByDeg()
					e:FireRhombusBullet(td,2,5.0)
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			state=0
		elseif state==4 then
			for i=1,5 do
				utls.Wait(20)
				for j=1,16 do
					e:FireBatsuBullet(j*(360/16)+(360/32)*(i%2),5,3.0)
				end
				utls.Wait(20)
				for j=1,4 do
					e:FireRhombusBullet(j*(360/4)+(360/8)*(i%2),2,5.0)
				end
			end
			state=0
		elseif state==5 then
			local cnt=0
			e.rota_vel=10
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%5==0 then
						e:FireTogeBullet(7,5.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%5==0 then
						local td = e:GetTargetDirectionByDeg()
						e:FireRhombusBullet(td,2,5.0)
					end
					cnt = cnt+1
					CrtnMgr:Yield()
				end
			end
			e.rota_vel=0
			e.rotation=0
			state=0
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function SuperWhite(e)
	local height = 200
	e.vr=0
	e.vh=0
	local rest_frame = 20
	local p_radius = e:GetBelongingPlanetRadius()
	local move_frame = 40
	
	local state=0
	local atk_cntr=8;
	local super_atk_cnt=10;
	
	while(true)do
		if state==0 then --待機
			for i=1,rest_frame do
				CrtnMgr:Yield()
			end
			e:StartHermiteMove(Vector2D(e:GetTargetT(),p_radius + height),Vector2D(0,0),Vector2D(0,0),move_frame)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			GameSound.PlaySoundByID("BossMove")
			if atk_cntr >= super_atk_cnt then
				state = 100
				atk_cntr=0;
			else
				state = math.random(1,10)
				atk_cntr = atk_cntr + 1
			end
		elseif state==1 then --回転連射
			for i=1,300 do
				if i%3==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
					e:FireBatsuBullet(-e.rotation+60,5,3.0)
					e:FireBatsuBullet(-e.rotation+120,5,3.0)
					e:FireBatsuBullet(-e.rotation+180,5,3.0)
					e:FireBatsuBullet(-e.rotation+240,5,3.0)
					e:FireBatsuBullet(-e.rotation+300,5,3.0)
				end
				e.rota_vel = e.rota_vel+0.1
				CrtnMgr:Yield()
			end
			local i=0
			while(e.rota_vel>0)do
				if i%3==0 then
					e:FireBatsuBullet(-e.rotation,5,3.0)
					e:FireBatsuBullet(-e.rotation+60,5,3.0)
					e:FireBatsuBullet(-e.rotation+120,5,3.0)
					e:FireBatsuBullet(-e.rotation+180,5,3.0)
					e:FireBatsuBullet(-e.rotation+240,5,3.0)
					e:FireBatsuBullet(-e.rotation+300,5,3.0)
				end
				e.rota_vel = e.rota_vel - 0.5
				i=i+1
				CrtnMgr:Yield()
			end
			e.rota_vel = 0
			state=0
		elseif state==2 then --追尾弾散布
			local shoot_num=16
			utls.Wait(2)
			for i=1,shoot_num do
				e:FireRhombusBullet(i*(360/shoot_num),2,5.0)
				utls.Wait(4)
			end
			state=0
		elseif state==3 then --囲い込みレーザー
			local shoot_num=5
			local ll_id = e:FireLaser(32,30,10000,255,255,255,120)
			local lr_id = e:FireLaser(32,30,10000,255,255,255,60)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			for i=1,shoot_num do
				utls.Wait(5)
				local l_id = e:FireLaser(30,10,30,255,255,255,(120-60)/(shoot_num+1)*i+60)
				while(e:GetObject(l_id)~=nil)do
					CrtnMgr:Yield()
				end
			end	
			e:EndObject(ll_id)
			e:EndObject(lr_id)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			state=0
		elseif state==4 then --囲い込み誘導弾
			local shoot_num=8
			local ll_id = e:FireLaser(32,30,10000,255,255,255,120)
			local lr_id = e:FireLaser(32,30,10000,255,255,255,60)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			for i=1,shoot_num do
				local td = e:GetTargetDirectionByDeg()
				e:FireRhombusBullet(td,2,5.0)
				utls.Wait(30)
			end	
			e:EndObject(ll_id)
			e:EndObject(lr_id)
			for i=1,60 do
				CrtnMgr:Yield()
			end
			state=0
		elseif state==5 then --蛇行
			e:StartHermiteMove(Vector2D(e.t+360,p_radius + height),Vector2D(0,0),Vector2D(0,0),180)
			local cnt=0
			local l_id = e:FireLaser(24,30,10000,255,255,255,90)
			while(e:IsAutoMoving())do
				e:GetObject(l_id).t=e.t
				CrtnMgr:Yield()
			end
			e:EndObject(l_id)
			state=0
		elseif state==6 then
			for i=1,10 do
				utls.Wait(15)
				for j=1,20 do
					e:FireBatsuBullet(j*(360/20)+(360/32)*(i%2),5,3.0)
				end
				e:FireTogeBullet(7,5.0)
				utls.Wait(15)
				for j=1,4 do
					e:FireRhombusBullet(j*(360/4)+(360/8)*(i%2),2,5.0)
				end
				e:FireTogeBullet(7,5.0)
			end
			state=0
		elseif state==7 then
			local cnt=0
			e.rota_vel=10
			local l_id = e:FireLaser(24,30,10000,255,255,255,90)
			for i=1,6 do
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height+100),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%3==0 then
						e:FireTogeBullet(7,5.0)
					end
					e:GetObject(l_id).t=e.t
					e:GetObject(l_id).r=e.r
					cnt = cnt+1
					CrtnMgr:Yield()
				end
				for j=1,10 do
					e:FireBatsuBullet(360/10*j,5,3.0)
				end
				e:StartHermiteMove(Vector2D(e.t+360/12,p_radius + height),Vector2D(0,0),Vector2D(0,0),20)
				while(e:IsAutoMoving())do
					if cnt%5==0 then
						local td = e:GetTargetDirectionByDeg()
						e:FireRhombusBullet(td,2,5.0)
					end
					e:GetObject(l_id).t=e.t
					e:GetObject(l_id).r=e.r
					cnt = cnt+1
					CrtnMgr:Yield()
				end
			end
			e:EndObject(l_id)
			e.rota_vel=0
			e.rotation=0
			state=0
		elseif state==8 then
			e:StartHermiteMove(Vector2D(e.t,p_radius +10),Vector2D(0,0),Vector2D(0,0),30)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			e:StartLinearMove(Vector2D(e.t+360,p_radius + 10),60)
			local cnt=0
			while(e:IsAutoMoving())do
				if cnt%5==0 then
					e:FireBatsuBullet(0,0,3.0)
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			state=0
		elseif state==9 then
			e:StartHermiteMove(Vector2D(e.t,p_radius + height/2),Vector2D(0,0),Vector2D(0,0),30)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
			e:StartLinearMove(Vector2D(e.t-360,p_radius + height/2),60)
			local cnt=0
			while(e:IsAutoMoving())do
				if cnt%5==0 then
					e:FireRhombusBullet(270,0.5,5.0)
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			state=0
		elseif state==100 then --連続攻撃(ワープ・連射)
			local wp_frame=30
			local t_b = e.t
--			local r_b = e.r
			for i=1,wp_frame do
				if i%2==0 then
					e.t=t_b
--					e.r=r_b
				else
					e.t=e:GetTargetT()
--					e.r=e:GetTargetR()
				end
				CrtnMgr:Yield()
			end
			e.t=e:GetTargetT()
--			e.r=e:GetTargetR()
			state=-1
		elseif state==-1 then --連続攻撃(ワープ・2重連射)
			for i=1,150 do
				if i%3==0 then
					e:FireBatsuBullet(270,10,3.0)
				end
				e.t=e:GetTargetT()
				CrtnMgr:Yield()
			end
			for i=1,30 do
				e.t=e:GetTargetT()
				CrtnMgr:Yield()
			end
			for i=1,150 do
				if i%3==0 then
					e:FireBatsuBullet(225,10,3.0)
					e:FireBatsuBullet(315,10,3.0)
				end
				if i%20==0 then
					e:FireRhombusBullet(270,5,5.0)
				end
				e.t=e:GetTargetT()
				CrtnMgr:Yield()
			end
			state=-2
		elseif state==-2 then --連続攻撃(囲い込み)
			local hsm_ang=90
			local t_l = e.t-hsm_ang
			local t_r = e.t+hsm_ang
			local t_c = e.t
			local ll_id = e:FireLaser(40,30,10000,255,255,255,90)
			local lr_id = e:FireLaser(40,30,10000,255,255,255,90)
			for i=1,190 do
				t_c = e:GetTargetT()
				t_l = t_c-hsm_ang
				t_r = t_c+hsm_ang
				e:GetObject(ll_id).t=t_l
				e:GetObject(lr_id).t=t_r
				
				if i%20==0 then
					local t_b=e.t
					e.t=t_l
					e:FireTogeBullet(10,5.0)
					e.t=t_r
					e:FireTogeBullet(10,5.0)
					e.t=t_b
				end
				if i%60==0 then
					local t_b=e.t
					e.t=t_c
					e:FireRhombusBullet(315,2,5.0)
					e:FireRhombusBullet(225,2,5.0)
					e:FireRhombusBullet(30,15,5.0)
					e:FireRhombusBullet(150,15,5.0)
					e.t=t_b
				end
				
				if i%3==0 then
					e.t=t_c
				elseif i%3==1 then
					e.t=t_l
				else
					e.t=t_r
				end
				hsm_ang = hsm_ang - 0.4
				CrtnMgr:Yield()
			end
			e:EndObject(ll_id)
			e:EndObject(lr_id)
			state=-3
		elseif state==-3 then --連続攻撃(乱射)
			e:StartHermiteMove(Vector2D(e.t,p_radius + height*2),Vector2D(0,0),Vector2D(0,0),40)
			while(e:IsAutoMoving())do
				CrtnMgr:Yield()
			end
--			e:StartLinearMove(Vector2D(e.t-360,p_radius + height*2),400)
			e.vh=-(math.pi*2/400)*(p_radius+height*2)
			local l_id={}
			for i=1,4 do
				l_id[i]=e:FireLaser(24,30,10000,255,255,255,90*i)
				e:GetObject(l_id[i]).rota_vel=1.5
			end
			e.rota_vel=1.5
			local cnt=0
			while(cnt<400)do
				if cnt%7==0 then
					for i=1,30 do
						local dir=360/30*(i+0.5)
						local b=math.random(0,5)
						if b<=4 then
							e:FireBatsuBullet(dir,5,3.0)
						else
							e:FireRhombusBullet(dir,2,5.0)
						end
					end
				elseif cnt%10==5 then
					e:FireTogeBullet(15,5.0)
				end
				for i=1,4 do
					e:GetObject(l_id[i]).t=e.t
				end
				cnt=cnt+1
				CrtnMgr:Yield()
			end
			for i=1,4 do
				e:EndObject(l_id[i])
			end
			e.rota_vel=0
			e.rotation=0
			e.vh=0
			state=0
		elseif state==10 then
			while(e.ground==false)do
				e:AddGravity()
				CrtnMgr:Yield()
			end
			utls.Wait(60)
			state=0
		else
			state=0
		end
		CrtnMgr:Yield()
	end
end

function Rainbow(e)

end

function MicroFly(e)
	local vt = 0.4
	local vr = 0.4
	local t = e.t
	local r = e.r
	while(true)do
		e.vr = 0
		if utls.InRange(t,e.t-2,e.t+2) == false then t = e.t end
		if utls.InRange(r,e.r-2,e.r+2) == false then r = e.r end
--		utls.MoveToT(e.t,e:GetTargetT(),1)
		local pur_t = e:GetTargetT()
		if ((pur_t - t + 360) % 360) < 180 then
			t = t + vt
		else
			t = t - vt
		end
		local pur_r = e:GetTargetR()
		r = r + (pur_r - r)/math.abs(pur_r - r) * vr
		e.t = t + math.random(-1,1)
		e.r = r + math.random(-1,1)
		CrtnMgr:Yield()
	end
end

--ライザ
function Raiza(e)
	--上陸前
	local cnt = 0
	local planet_radius = e:GetBelongingPlanetRadius() --惑星の半径
	local height_from_planet = 150 --高度
	local laser_time = 40 --レーザーの持続時間
	local laser_suspention = 30 --レーザーの発射猶予
	local rest_timep = 30 --発射前に休む時間
	local rest_time = 60 --発射後に休む時間
	local move_frame=60
	local attack_num=3 --攻撃回数
	e:StartAction("idol")
	e.vh=0
	e.vr=0
	--惑星表面からから150の位置まで移動
	e:StartHermiteMove(Vector2D(e.t,planet_radius + height_from_planet),Vector2D(0,0),Vector2D(0,0),move_frame)
	--移動終了まで待機
	while(e:IsAutoMoving())do
		CrtnMgr:Yield()
	end
	for i=1,attack_num do
		--プレイヤーの頭上まで移動
		e:StartHermiteMove(Vector2D(e.t+e:GetTargetTFromMe(),e.r),Vector2D(0,300),Vector2D(0,-300),move_frame)
		--移動終了まで待機
		while(e:IsAutoMoving())do
			CrtnMgr:Yield()
		end
		--待機
		for i=0,rest_timep do
			CrtnMgr:Yield()
		end
		--レーザー発射
		e:FireLaser(24,laser_suspention,laser_time,0,255,255,90)
		--レーザー待機
		for i=0,laser_time + laser_suspention*2 do
			CrtnMgr:Yield()
		end
		--待機
		for i=0,rest_time do
			CrtnMgr:Yield()
		end
	end
	--落下
	while(e.ground==false)do
		e:AddGravity()
		CrtnMgr:Yield()
	end
	--上陸後
	e:Landed();
	while(true)do
		e:Friction()
		e:AddGravity()
		CrtnMgr:Yield()
	end
end
--トゲラ
function Togera(e)
	local state=0
	local inc_cnt=0
	local move_frame=60
	local move_speed=100
	local move_dir=0
	while(true)do
		if state==0 then
			if math.random(1,100) == 1 then e:StartAction("wink") end
			if e:GetTargetDistanceT()<=45 then
				state=1
				e:StartAction("toge")
				e.vr=6
				if e:GetTargetTFromMe()>=0 then
					move_dir=1
					e:ChangeDirection(Enemy.RIGHT)
				else
					move_dir=-1
					e:ChangeDirection(Enemy.LEFT)
				end
			end
		elseif state==1 then
			if e.ground==true and e:CheckActionPlaying()==false then
			e:AddForceToPlanet(e.t,500)
				state=2
			end
		elseif state==2 then
			e.t= e.t + move_dir * move_speed/e.r
			e.rotation = e.rotation +  move_dir * move_speed*(e.r/16)/e.r
			if inc_cnt>=move_frame then
				e:StartReverseAction("toge")
				state=3
				e.rotation=0
				inc_cnt=0
			end
			inc_cnt = inc_cnt + 1
		elseif state==3 then
			if e:CheckActionPlaying()==false then
				state=0
				e:StartAction("idol")
			end
		end
	
		e:AddGravity();
		CrtnMgr:Yield();
	end
end
--ザクル
function Zakuru(e)
	local rf=1
	if e.vh<0
		then e:ChangeDirection(Enemy.LEFT)
		rf=-1
	end
	--上陸前
	local cnt = 0
	e:StartAction("idol")
	e.rotation = math.deg(math.atan2(e.vh,e.vr))-90
	while(e.ground==false)do
--		if cnt%60==0 then e:FireRotatingBullet("EnemyBulletSquare",270,3,3.0,10) end
--		if cnt%60==0 then e:FireRotatingBullet("EnemyBulletOval",270,3,3.0,10) end
		e.rotation = cnt*10*rf
		cnt=cnt+1;
		CrtnMgr:Yield()
	end
	--上陸後
	e:Landed();
	--掘削しつつ巡回
	while(true)do
		e.rotation = e.rotation + e.vh/16/math.pi*180
		e:AddGravity();
		e:AddForceToPlanet(e.t,100)
		CrtnMgr:Yield()
	end
end
