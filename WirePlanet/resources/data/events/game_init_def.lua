--�V�X�e���֐�
--�������[���݊m�F(false:���� true:����)
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

--�������֐�(�Q�[���|�C���^)(������0,���s��-1��Ԃ�)

--�ʏ�X�e�[�W������
function NormalStageInitializer(g)
	g:SetupBackGround()
	g:SetupInformationArea()
	g:ActivateGameComponent("All",true)
	g:ChangeCameraTargetFromEveChr("Player",false)
	return 0
end

--�X�^�b�t���[��������
function StaffRollInitializer(g)
	return 0
end
