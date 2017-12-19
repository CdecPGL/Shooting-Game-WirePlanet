-------�V�X�e���֐�(�V�X�e������Ă΂�܂�)-------
--�C�x���g���݊m�F(false:���� true:����)
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

------�R���[�`���Ǘ�----------------
require("coroutinemanager")

CrtnMgr = coroutinemanager.CoroutineManager.new()


--���o��
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
--		game:PrintErrorMessage("�C�x���g���쐬����܂����BName:" ..event_name .." ID:"..id .."\n")
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

-----�O���[�o���ϐ�-----
current_wave=0; --���݂̔g�ԍ�
stop_game_process_flag=false; --�Q�[���i�s��~�t���O

--������
function Initialize()
	current_wave=0
	stop_game_process_flag=false
end

--����
--[[�X�e�[�W�ԍ��ɂ���
-1:�e�X�g�X�e�[�W
0:Tutorial
1-5:�ʏ�X�e�[�W
]]

--����������
math.randomseed(os.time())

--Util�֐�--
utils = {}
--�Q�[���i�s�ĊJ�܂őҋ@����
utils.WaitGameResume = function()
	while(stop_game_process_flag)do
		CrtnMgr:Yield()
	end
end
--�Q�[���i�s��~
utils.StopGameProcess = function()
	stop_game_process_flag=true
end
--�Q�[���i�s�ĊJ
utils.ResumeGameProcess = function()
	stop_game_process_flag=false
end

--�w��t���[���ҋ@
utils.Wait = function(f)
	for i=1,f do
		CrtnMgr:Yield()
	end
end

--�ʏ�̃��x���X�^�[�g����(�Q�[���|�C���^�A���x���A�G�����ԍ�)
utils.NormalStartLevelProcess = function(g,args)
	local level = args[1]
	local egen_num = args[2]
	--�G��������
--	GameSystem.PrintToConsole("seg\n")
	g:SetEnemyGeneration(egen_num);
--	GameSystem.PrintToConsole("slce\n")
	g:StartLevelChangeEffect(level);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("eslce\n")
	--�G���������̊�����҂�
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
--GameSystem.PrintToConsole("eseg\n")	
	--�G���ʒm�Ɛ����J�n
	local en=g:GetGenerateEnemyCount(egen_num)
--	GameSystem.PrintToConsole("tekisuutuutid\n")
	g:CreateSingleLineWindow(en.."�̂̓G���ڋߒ��ł��I",120,"Bottom");
	g:StartEnemyGeneration();
--	GameSystem.PrintToConsole("startenemygene\n")
	if GameSound.IsAssignedMusicPlaying("NormalBGM")==false then GameSound.PlayMusic("NormalBGM") end
end

--�X�e�[�W�J�n2
utils.LaterStageStartLevelProcess = function(g,args)
	local level = args[1]
	local egen_num = args[2]
	--�G��������
	g:SetEnemyGeneration(egen_num);
	g:StartLevelChangeEffect(level);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--�G���������̊�����҂�
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--�G���ʒm�Ɛ����J�n
	local en=g:GetGenerateEnemyCount(egen_num)
	g:CreateSingleLineWindow(en.."�̂̓G���ڋߒ��ł��I",120,"Bottom");
	g:StartEnemyGeneration();
	if GameSound.IsAssignedMusicPlaying("LaterStage")==false then GameSound.PlayMusic("LaterStage") end
end

--�ŏI�X�e�[�W�J�n
utils.StartLastLevelProcess = function(g,args)
	local egen_num = args[1]
	--�G��������
	g:SetEnemyGeneration(egen_num);
	g:StartLevelChangeEffect(-5);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--�G���������̊�����҂�
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--�G���ʒm�Ɛ����J�n
	local en=g:GetGenerateEnemyCount(egen_num)
	g:CreateSingleLineWindow(en.."�̂̓G���ڋߒ��ł��I",120,"Bottom");
	g:StartEnemyGeneration();
end

--�{�X�J�n(�Q�[���|�C���^�A�{�XID)
utils.BossStart = function(g,args)
	local boss = args[1]
	g:StartLevelChangeEffect(-6);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	local id = g:CreateEnemy(boss,"Boss",Vector2D(0,700))--�{�X�쐬
	local b = g:GetEventCharacter("Boss")
	b:ChangeMode(Character.StandAlone)
	g:DetachEventCharacter("Boss")
	if GameSound.IsAssignedMusicPlaying("Boss")==false then GameSound.PlayMusic("Boss") end
end

--�{�X�J�n2
utils.BossStart2 = function(g,args)
	local boss = args[1]
	g:StartLevelChangeEffect(-6);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	local id = g:CreateEnemy(boss,"Boss",Vector2D(0,700))--�{�X�쐬
	local b = g:GetEventCharacter("Boss")
	b:ChangeMode(Character.StandAlone)
	g:DetachEventCharacter("Boss")
	if GameSound.IsAssignedMusicPlaying("Boss2")==false then GameSound.PlayMusic("Boss2") end
	g:SetPauseKind(Game.OnlyContinue)
end

--���X�{�X�J�n
utils.LastBossStart = function(g,args)
	local boss = args[1]
end

--�E���{�X�J�n
utils.UraBossStart = function(g,args)
	local boss = args[1]
end

--�X�e�[�W���Ƃ̊J�n�֐��ƈ���
stage_start_func_list={}
--wave1~3(1-3)
for i=1,3 do
	stage_start_func_list[i] = {func = utils.NormalStartLevelProcess , params = {i,i+99} , bg=1}
end
--�{�X1(4)
stage_start_func_list[4] = {func = utils.BossStart, params={"SuperGreen"}, bg=1}
--wave4-6(5-7)
for i=4,6 do
	stage_start_func_list[i+1] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=2}
end
--�{�X2(8)
stage_start_func_list[8] = {func = utils.BossStart, params={"SuperSkyblue"}, bg=2}
--wave7-10(9-12)
for i=7,10 do
	stage_start_func_list[i+2] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=3}
end
--�{�X3(13)
stage_start_func_list[13] = {func = utils.BossStart, params={"SuperRed"}, bg=3}
--wave11-14(14-17)
for i=11,14 do
	stage_start_func_list[i+3] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=4}
end
--�{�X4(18)
stage_start_func_list[18] = {func = utils.BossStart, params={"SuperPurple"}, bg=4}
--wave15-19(19-23)
for i=15,19 do
	stage_start_func_list[i+4] = {func = utils.NormalStartLevelProcess , params = {i,i+99}, bg=5}
end
--�{�X5(24)
stage_start_func_list[24] = {func = utils.BossStart, params={"SuperPink"}, bg=5}
--wave20-29(25-34)
for i=20,29 do
	stage_start_func_list[i+5] = {func = utils.LaterStageStartLevelProcess , params = {i,i+99}, bg=6}
end
--�{�X6(35)
stage_start_func_list[35] = {func = utils.BossStart2, params={"SuperWhite"}, bg=6}
--���X�{�X(36)
stage_start_func_list[36] = {func = utils.LastBossStart, params={"Rainbow"}, bg=7}
--[[
--wave36-50(43-57)
for i=36,50 do
	stage_start_func_list[i+7] = {func = utils.NormalStartLevelProcess , params = {i,i+99}}
end
--lastwave(58)
stage_start_func_list[58] = {func = utils.StartLastLevelProcess, params={51 + 99}}
--���{�X(59)
stage_start_func_list[59] = {func = utils.UraBossStart, params={"UraBoss"}}
]]
-----�w��X�e�[�W�J�n�֐�(0�Ő��� -1�Ŏ��s)
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

--�N���A����(0:���N���A 1:�m�[�}���N���A2:���S�N���A)
utils.CheckGameCleared = function()
	if stage_start_func_list[current_wave+1]==nil then return 2 end
	if current_wave==35 then return 1 end
	return 0
end

--�X�e�[�W������
utils.InitStage = function(g)
	--�L�����z�u
	local player_b = g:GetEventCharacter("Player");
	player_b.t=0
	player_b:ChangeDirection(Character.RIGHT);
	--�J�����ύX
	g:ChangeCameraMode("Normal",true)
	--�v���C���[�𑀍�\��
	g:SetPlayerOperableFlag(1)
	--���x���J�n����
--	utils.NormalStartLevelProcess(g,param,param+99)
	
--	g:CreateSingleEventTriggerWithParam("LevelCleared","LevelControl",param)
end

------�C�x���g��`-------
--�����ɂ͔z�񂪓n����A[g]�ɂ̓Q�[���N���X�̃|�C���^�A[p1]�ɂ̓p�����[�^���L������Ă��܂��B

--�Q�[���J�n���ɌĂяo�����C�x���g(-1:�e�X�g�X�e�[�W�A0:Tutorial�A1-5:�ʏ�X�e�[�W)
function StartEvent(arg)
	local g = arg.g
	local param = arg.p1
	if param < -1 then 
		g:PrintErrorMessage("StartEvent:��`����Ă��Ȃ��J�n�ԍ��ł��B:" ..param);
	elseif param==-1 then --�e�X�g�X�e�[�W
		g:CreateSingleEventTrigger("Always","StartTestStage")
	else --�ʏ�X�e�[�W
		utils.InitStage(g)
		
		--�R���e�B�j���[�ɂ��J�n�łȂ�������|�[�Y�̐������o��
		if g:CheckContinued()==false then
			utils.Wait(30)
			g:CreateSingleLineWindowWS("--SPACE�L�[�Ń|�[�Y�ƃw���v--",180,"Center")
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

--�Q�[���I�[�o�[�m�F
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

--�Q�[���I�[�o�[�C�x���g
function GameOver(arg)
	local g = arg.g
	local param = arg.p1
	--�Q�[���i�s��~�����������߂�
	if stop_game_process_flag==true then return end
	--�Q�[���i�s��~
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
		--�X�R�A�W�v
		g:StartWaveEndScoreCounting()
		while(g:IsCountingWaveEndScore())do
			CrtnMgr:Yield()
		end
		--���ʏW�v
		g:StartResultCounting(0)
		while(g:IsCountingResult())do
			CrtnMgr:Yield()
		end
		g:GameEnd()
	end
end

--�e�X�g�X�e�[�W�J�n
function StartTestStage(arg)
	local g = arg.g
	local param = arg.p1
	local player = g:GetEventCharacter("Player")
	player:ChangeMode(Character.StandAlone)
	g:ChangeBackGround(1)
	--�v���C���[�𑀍�\��
	g:SetPlayerOperableFlag(1)
	--���x���J�n����
	--�G��������
	g:SetEnemyGeneration(1000);
	g:StartLevelChangeEffect(-3);
	--���x���ύX�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
	--�G���������̊�����҂�
	while(g:CheckEnemyGeneratePrepairing()==true)do
		CrtnMgr:Yield()
	end
	--�G���ʒm�Ɛ����J�n
	local en=g:GetGenerateEnemyCount(1000)
	g:CreateSingleLineWindow(en.."�̂̓G���ڋߒ��ł��I",120,"Bottom");
	g:StartEnemyGeneration();
	g:CreateSingleEventTrigger("LevelCleared","StartTestStage")
	if gagaga==nil then 
		g:CreateSingleEventTrigger("Always","CheckGameOver") 
		gagaga=1
	end
end

--�`���[�g���A���J�n
function StartTutorial(arg)
	local g = arg.g
	local param = arg.p1
	--���y�Đ�
	g:PlayMusic("Tutorial");
	g:StartLevelChangeEffect(0);
end

--�`���[�g���A���C�x���g
function TutorialEvent(arg)
end

--���x������C�x���g
function LevelControl(arg)
	local g = arg.g
	local level = arg.p1
	local igc=utils.CheckGameCleared()
	--�Q�[���i�s�ĊJ�ҋ@
	utils.WaitGameResume()
	--�m�[�}���N���A������
	if igc==1 then
		--�Q�[���i�s��~
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
		--���ʏW�v
		g:StartResultCounting(1)
		while(g:IsCountingResult())do
			CrtnMgr:Yield()
		end
		g:GameCleared()
		return
	end
	--�N���A�G�t�F�N�g
	g:StartLevelChangeEffect(-4);
	--�G�t�F�N�g�̏I����҂�
	while(g:CheckLevelChangeEffectPlaying())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("CLFE\n");
	--�X�R�A�W�v
	g:StartWaveEndScoreCounting()
--	GameSystem.PrintToConsole("swesc\n");
	while(g:IsCountingWaveEndScore())do
		CrtnMgr:Yield()
	end
--	GameSystem.PrintToConsole("eswesc\n");
--	GameSystem.PrintToConsole("ss\n");
	--�G�l���M�[�ۑ�
	g:SavePlayerEnergy()
	--���x���J�n����
	utils.StartStage(g,level + 1)
--	GameSystem.PrintToConsole("ess\n");
	g:CreateSingleEventTriggerWithParam("LevelCleared","LevelControl",level+1)
end
