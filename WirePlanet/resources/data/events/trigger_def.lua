-----------システム関数---------
--トリガー存在確認(false:無い true:ある)
function CheckTriggerNameExistence(trigger_name)
	local f = _G[trigger_name];
	if f==nil then
		return false
	elseif type(f) ~= "function" then
		return false
	else
		return true
	end
end

---トリガー実行(エラーで負数、条件を満たしてなかったら0、条件を満たしてたら1を返す)。システムから呼ばれます。
function ExcuteTrigger(trigger_name,game)
	local f = _G[trigger_name];
	if f==nil then
		return -1
	elseif type(f) ~= "function" then
		return -2
	else
		local res = f(game)
		if res==nil then
			return -3
		elseif res==false then
			return 0
		else
			return 1
		end
	end
end
-------------------------------------

--------トリガー定義(引数にはgameポインタが渡されます)-----

--レベル変更エフェクトが終わったら
function LevelChangeEffectEnded(g)
	if g:CheckLevelChangeEffectPlaying()==true then
		return false
	else 
		return true
	end
end

--敵生成が終わったら
function EnemyGeneratingEnded(g)
	if g:CheckEnemyGenerating()==true then
		return false
	else 
		return true
	end
end
--メッセージウインドウがだったら
function MessageWindowCountZero(g)
	return g:CheckMessageWindowCountZero()
end

--レベルクリアしたら
function LevelCleared(g)
	if g:CheckEnemyGenerating()==false and g:CheckEnemyCountZero()==true then
		return true
	else
		return false
	end
end

function Always(g)
	return true
end