-------システム関数(システムから呼ばれます)-------
--イベント存在確認(false:無い true:ある)
function CheckEventNameExistence(name)
	local f = _G[name];
	if f==nil then
		return false
	elseif type(f) ~= "function" then
		return false
	else
		return true
	end
end

------コルーチン管理----------------
require("coroutinemanager")

CrtnMgr = coroutinemanager.CoroutineManager.new()


--情報出力
function Dump()
    GameSystem.PrintToConsole("---EventDefDump---\n")
    CrtnMgr:Dump()
end

function Process()
--	GameSystem.PrintToConsole("EVP-0\n")
	CrtnMgr:Process()
--	GameSystem.PrintToConsole("EVP-1\n")
end

function CreateEvent(event_name,game,param)
--	local global_table = getfenv()
	local f = _G[event_name];
	if f==nil then
		return -1
	elseif type(f) ~= "function" then
		return -2
	else
		local arg = {g = game,p1 = param}
		local id = CrtnMgr:CreateCoroutine(f,arg)
--		game:PrintErrorMessage("イベントが作成されました。Name:" ..event_name .." ID:"..id .."\n")
		return id
	end
end

function RecycleEvent(event_id)
	return CrtnMgr:InvalidateCoroutine(event_id)
end

function InitEveMgr()
    CrtnMgr:SetDumpFunction(GameSystem.PrintToConsole)
	CrtnMgr:Init()
	return 0
end

function GetCount()
	return CrtnMgr:GetCount()
end
-----------------------------------
----------------------------------------

-----グローバル変数-----
current_wave=0; --現在の波番号
stop_game_process_flag=false; --ゲーム進行停止フラグ

--初期化
function Initialize()
	current_wave=0
	stop_game_process_flag=false
end

--メモ
--[[ステージ番号について
-1:テストステージ
0:Tutorial
1-5:通常ステージ
]]

--乱数初期化
math.randomseed(os.time())

--Util関数--
utils = {}
--ゲーム進行再開まで待機する
utils.WaitGameResume = function()
	while(stop_game_process_flag)do
		CrtnMgr:Yield()
	end
end
--ゲーム進行停止
utils.StopGameProcess = function()
	stop_game_process_flag=true
end
--ゲーム進行再開
utils.ResumeGameProcess = function()
	stop_game_process_flag=false
end

--指定フレーム待機
utils.Wait = function(f)
	for i=1,f do
		CrtnMgr:Yield()
	end
end

--通常のレベルスタート処理(ゲームポインタ、レベル、敵生成番号)
utils.NormalStartLevelProcess = function(g,args)
	local level = args[1]
	local egen_num = args[2]
	--敵生成準備
--	GameSystem.PrintToConsole("seg\n")
	g:SetEnemyGeneration(egen_num);
--	GameSystem.PrintToConsole("slce\n")
	g:StartLevelChangeEffect(level);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("eslce\n")
	--敵生成処理の完了を待つ
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
--GameSystem.PrintToConsole("eseg\n")	
	--敵数通知と生成開始
	local en=g:GetGenerateEnemyCount(egen_num)
--	GameSystem.PrintToConsole("tekisuutuutid\n")
	g:CreateSingleLineWindow(en.."体の敵が接近中です！",120,"Bottom");
	g:StartEnemyGeneration();
--	GameSystem.PrintToConsole("startenemygene\n")
	if GameSound.IsAssignedMusicPlaying("NormalBGM")==false then GameSound.PlayMusic("NormalBGM") end
end

--ステージ開始2
utils.LaterStageStartLevelProcess = function(g,args)
	local level = args[1]
	local egen_num = args[2]
	--敵生成準備
	g:SetEnemyGeneration(egen_num);
	g:StartLevelChangeEffect(level);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--敵生成処理の完了を待つ
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--敵数通知と生成開始
	local en=g:GetGenerateEnemyCount(egen_num)
	g:CreateSingleLineWindow(en.."体の敵が接近中です！",120,"Bottom");
	g:StartEnemyGeneration();
	if GameSound.IsAssignedMusicPlaying("LaterStage")==false then GameSound.PlayMusic("LaterStage") end
end

--最終ステージ開始
utils.StartLastLevelProcess = function(g,args)
	local egen_num = args[1]
	--敵生成準備
	g:SetEnemyGeneration(egen_num);
	g:StartLevelChangeEffect(-5);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--敵生成処理の完了を待つ
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--敵数通知と生成開始
	local en=g:GetGenerateEnemyCount(egen_num)
	g:CreateSingleLineWindow(en.."体の敵が接近中です！",120,"Bottom");
	g:StartEnemyGeneration();
end

--ボス開始(ゲームポインタ、ボスID)
utils.BossStart = function(g,args)
	local boss = args[1]
	g:StartLevelChangeEffect(-6);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	local id = g:CreateEnemy(boss,"Boss",Vector2D(0,700))--ボス作成
	local b = g:GetEventCharacter("Boss")
	b:ChangeMode(Character.StandAlone)
	g:DetachEventCharacter("Boss")
	if GameSound.IsAssignedMusicPlaying("Boss")==false then GameSound.PlayMusic("Boss") end
end

--ボス開始2
utils.BossStart2 = function(g,args)
	local boss = args[1]
	g:StartLevelChangeEffect(-6);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	local id = g:CreateEnemy(boss,"Boss",Vector2D(0,700))--ボス作成
	local b = g:GetEventCharacter("Boss")
	b:ChangeMode(Character.StandAlone)
	g:DetachEventCharacter("Boss")
	if GameSound.IsAssignedMusicPlaying("Boss2")==false then GameSound.PlayMusic("Boss2") end
	g:SetPauseKind(Game.OnlyContinue)
end

--ラスボス開始
utils.LastBossStart = function(g,args)
	local boss = args[1]
end

--ウラボス開始
utils.UraBossStart = function(g,args)
	local boss = args[1]
end

--ステージごとの開始関数と引数
stage_start_func_list={}
--wave1~3(1-3)
for i=1,3 do
	stage_start_func_list[i] = {func = utils.NormalStartLevelProcess , params = {i,i+99} , bg=1}
end
--ボス1(4)
stage_start_func_list[4] = {func = utils.BossStart, params={"SuperGreen"}, bg=1}
--wave4-6(5-7)
for i=4,6 do
	stage_start_func_list[i+1] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=2}
end
--ボス2(8)
stage_start_func_list[8] = {func = utils.BossStart, params={"SuperSkyblue"}, bg=2}
--wave7-10(9-12)
for i=7,10 do
	stage_start_func_list[i+2] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=3}
end
--ボス3(13)
stage_start_func_list[13] = {func = utils.BossStart, params={"SuperRed"}, bg=3}
--wave11-14(14-17)
for i=11,14 do
	stage_start_func_list[i+3] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=4}
end
--ボス4(18)
stage_start_func_list[18] = {func = utils.BossStart, params={"SuperPurple"}, bg=4}
--wave15-19(19-23)
for i=15,19 do
	stage_start_func_list[i+4] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=5}
end
--ボス5(24)
stage_start_func_list[24] = {func = utils.BossStart, params={"SuperPink"}, bg=5}
--wave20-29(25-34)
for i=20,29 do
	stage_start_func_list[i+5] = {func = utils.LaterStageStartLevelProcess , params = {i,i+99}, bg=6}
end
--ボス6(35)
stage_start_func_list[35] = {func = utils.BossStart2, params={"SuperWhite"}, bg=6}
--ラスボス(36)
stage_start_func_list[36] = {func = utils.LastBossStart, params={"Rainbow"}, bg=7}
--[[
--wave36-50(43-57)
for i=36,50 do
	stage_start_func_list[i+7] = {func = utils.NormalStartLevelProcess , params = {i,i+99}}
end
--lastwave(58)
stage_start_func_list[58] = {func = utils.StartLastLevelProcess, params={51 + 99}}
--裏ボス(59)
stage_start_func_list[59] = {func = utils.UraBossStart, params={"UraBoss"}}
]]
-----指定ステージ開始関数(0で成功 -1で失敗)
utils.StartStage = function(g,stage_num)
	local ssf = stage_start_func_list[stage_num]
	if ssf == nil then
		return -1
	end
	ssf.func(g,ssf.params)
	if ssf.bg~=nil then g:ChangeBackGround(ssf.bg) end
	current_wave = stage_num
	return 0
end

--クリア判定(0:未クリア 1:ノーマルクリア2:完全クリア)
utils.CheckGameCleared = function()
	if stage_start_func_list[current_wave+1]==nil then return 2 end
	if current_wave==35 then return 1 end
	return 0
end

--ステージ初期化
utils.InitStage = function(g)
	--キャラ配置
	local player_b = g:GetEventCharacter("Player");
	player_b.t=0
	player_b:ChangeDirection(Character.RIGHT);
	--カメラ変更
	g:ChangeCameraMode("Normal",true)
	--プレイヤーを操作可能に
	g:SetPlayerOperableFlag(1)
	--レベル開始処理
--	utils.NormalStartLevelProcess(g,param,param+99)
	
--	g:CreateSingleEventTriggerWithParam("LevelCleared","LevelControl",param)
end

------イベント定義-------
--引数には配列が渡され、[g]にはゲームクラスのポインタ、[p1]にはパラメータが記憶されています。

--ゲーム開始時に呼び出されるイベント(-1:テストステージ、0:Tutorial、1-5:通常ステージ)
function StartEvent(arg)
	local g = arg.g
	local param = arg.p1
	if param < -1 then 
		g:PrintErrorMessage("StartEvent:定義されていない開始番号です。:" ..param);
	elseif param==-1 then --テストステージ
		g:CreateSingleEventTrigger("Always","StartTestStage")
	else --通常ステージ
		utils.InitStage(g)
		
		--コンティニューによる開始でなかったらポーズの説明を出す
		if g:CheckContinued()==false then
			utils.Wait(30)
			g:CreateSingleLineWindowWS("--SPACEキーでポーズとヘルプ--",180,"Center")
			utils.Wait(210)
			--[[g:StartLevelChangeEffect(-8);
			while(g:CheckLevelChangeEffectPlaying())do
				CrtnMgr:Yield()
			end
			utils.Wait(60)]]
		end
		
		utils.StartStage(g,param)
		g:CreateSingleEventTrigger("Always","CheckGameOver")
		g:CreateSingleEventTriggerWithParam("LevelCleared","LevelControl",param);
	end
end

--ゲームオーバー確認
function CheckGameOver(arg)
	local g = arg.g
	local param = arg.p1
	while(g:CheckPlanetDanger()==false and g:CheckUsingPlayerDead()==false)do
		CrtnMgr:Yield()
	end
	if g:CheckUsingPlayerDead()==false then g:StartGameScreenEffect("Alert") end
	while(g:CheckPlanetDestroy()==false and g:CheckUsingPlayerDead()==false)do
		CrtnMgr:Yield()
	end
	g:CreateSingleEventTrigger("Always","GameOver");
end

--ゲームオーバーイベント
function GameOver(arg)
	local g = arg.g
	local param = arg.p1
	--ゲーム進行停止中だったらやめる
	if stop_game_process_flag==true then return end
	--ゲーム進行停止
	utils.StopGameProcess()
	g:SetPauseKind(Game.CantReturn)
	GameSound.StopMusic()
	utils.Wait(120)
	GameSound.PlayMusic("GameOver")
	g:GameOvered()
	g:SetPlayerOperableFlag(0)
	g:ActivateGameComponent("InformationArea",false)
--	g:GetEventCharacter("UsingPlayer")
	g:StartFullScreenGraphDrawing("GameOverScreen")
	for i=1,150 do
		CrtnMgr:Yield()
	end
	g:CreateTextWindow(1)
	while(g:CheckTalkWindowActive())do
		CrtnMgr:Yield()
	end
	local slctd_idx = g:GetLastSelectedIndexMW()
	if slctd_idx==0 then
		g:ContinueGame(current_wave)
	elseif slctd_idx==1 then
		--スコア集計
		g:StartWaveEndScoreCounting()
		while(g:IsCountingWaveEndScore())do
			CrtnMgr:Yield()
		end
		--結果集計
		g:StartResultCounting(0)
		while(g:IsCountingResult())do
			CrtnMgr:Yield()
		end
		g:GameEnd()
	end
end

--テストステージ開始
function StartTestStage(arg)
	local g = arg.g
	local param = arg.p1
	local player = g:GetEventCharacter("Player")
	player:ChangeMode(Character.StandAlone)
	g:ChangeBackGround(1)
	--プレイヤーを操作可能に
	g:SetPlayerOperableFlag(1)
	--レベル開始処理
	--敵生成準備
	g:SetEnemyGeneration(1000);
	g:StartLevelChangeEffect(-3);
	--レベル変更エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--敵生成処理の完了を待つ
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--敵数通知と生成開始
	local en=g:GetGenerateEnemyCount(1000)
	g:CreateSingleLineWindow(en.."体の敵が接近中です！",120,"Bottom");
	g:StartEnemyGeneration();
	g:CreateSingleEventTrigger("LevelCleared","StartTestStage")
	if gagaga==nil then 
		g:CreateSingleEventTrigger("Always","CheckGameOver") 
		gagaga=1
	end
end

--チュートリアル開始
function StartTutorial(arg)
	local g = arg.g
	local param = arg.p1
	--音楽再生
	g:PlayMusic("Tutorial");
	g:StartLevelChangeEffect(0);
end

--チュートリアルイベント
function TutorialEvent(arg)
end

--レベル制御イベント
function LevelControl(arg)
	local g = arg.g
	local level = arg.p1
	local igc=utils.CheckGameCleared()
	--ゲーム進行再開待機
	utils.WaitGameResume()
	--ノーマルクリアしたら
	if igc==1 then
		--ゲーム進行停止
		utils.StopGameProcess()
		g:SetPauseKind(Game.CantReturn)
		g:StartLevelChangeEffect(-7);
		while(g:CheckLevelChangeEffectPlaying())do
			CrtnMgr:Yield()
		end
		GameSound.PlayNoLoopMusic("GameClear")
		g:StartWaveEndScoreCounting()
		while(g:IsCountingWaveEndScore())do
			CrtnMgr:Yield()
		end
		--結果集計
		g:StartResultCounting(1)
		while(g:IsCountingResult())do
			CrtnMgr:Yield()
		end
		g:GameCleared()
		return
	end
	--クリアエフェクト
	g:StartLevelChangeEffect(-4);
	--エフェクトの終了を待つ
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("CLFE\n");
	--スコア集計
	g:StartWaveEndScoreCounting()
--	GameSystem.PrintToConsole("swesc\n");
	while(g:IsCountingWaveEndScore())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("eswesc\n");
--	GameSystem.PrintToConsole("ss\n");
	--エネルギー保存
	g:SavePlayerEnergy()
	--レベル開始処理
	utils.StartStage(g,level + 1)
--	GameSystem.PrintToConsole("ess\n");
	g:CreateSingleEventTriggerWithParam("LevelCleared","LevelControl",level+1)
end
