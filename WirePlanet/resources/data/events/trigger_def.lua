-----------�V�X�e���֐�---------
--�g���K�[���݊m�F(false:���� true:����)
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

---�g���K�[���s(�G���[�ŕ����A�����𖞂����ĂȂ�������0�A�����𖞂����Ă���1��Ԃ�)�B�V�X�e������Ă΂�܂��B
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

--------�g���K�[��`(�����ɂ�game�|�C���^���n����܂�)-----

--���x���ύX�G�t�F�N�g���I�������
function LevelChangeEffectEnded(g)
	if g:CheckLevelChangeEffectPlaying()==true then
		return false
	else 
		return true
	end
end

--�G�������I�������
function EnemyGeneratingEnded(g)
	if g:CheckEnemyGenerating()==true then
		return false
	else 
		return true
	end
end
--���b�Z�[�W�E�C���h�E����������
function MessageWindowCountZero(g)
	return g:CheckMessageWindowCountZero()
end

--���x���N���A������
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