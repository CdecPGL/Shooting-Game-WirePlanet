--システム関数
--初期化ー存在確認(false:無い true:ある)
function CheckInitializerExistence(name)
	local f = _G[name];
	if f==nil then
		return false
	elseif type(f) ~= "function" then
		return false
	else
		return true
	end
end

--初期化関数(ゲームポインタ)(成功で0,失敗で-1を返す)

--通常ステージ初期化
function NormalStageInitializer(g)
	g:SetupBackGround()
	g:SetupInformationArea()
	g:ActivateGameComponent("All",true)
	g:ChangeCameraTargetFromEveChr("Player",false)
	return 0
end

--スタッフロール初期化
function StaffRollInitializer(g)
	return 0
end
