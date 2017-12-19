--http://hyakuren-soft.sakura.ne.jp/games_html/lua_coroutine_page.html���Q��
--2014/11/4 Version 2.0 �s��C��
--�R���[�`���̏I�����͉����Ԃ��Ă͂Ȃ�Ȃ�
module( "coroutinemanager", package.seeall )

CoroutineManager = {}

--�R���[�`���}�l�[�W��
function CoroutineManager:new()
	local members = {
		_coroutines = {},
		_size = 0,
		_id_counter = 1,
		_id_table = {},
		_dump_func = print
	}
	CoroutineManager.__index = CoroutineManager
	setmetatable(members,CoroutineManager)
	return members
end

--�R���[�`���쐬
function CoroutineManager:CreateCoroutine(func,arg)
--    self:Dump()
	local index = self._size + 1
	local id 
	local self_function --�쐬��
	
	if self._size >= #self._coroutines then
	-- �R���[�`���̐V�K�쐬
	self._coroutines[index] = {
		_func = func,
		_arg = arg,
		_co = nil,
		_id = self._id_counter,
		_wakeup_ticks = false
	}	
	self_function = self._coroutines[index]
	id = self._id_counter
	self._id_counter = self._id_counter + 1
	
	else
		-- �R���[�`���̍ė��p
		self_function = self._coroutines[index]
		self_function._func = func
		self_function._arg = arg
		self_function._wakeup_ticks = false
		id = self_function._id
	end
	
	--�R���[�`�������쐬��������쐬����
	if self_function._co==nil then
    	self_function._co =
		coroutine.create(function()
			while true do
		   		self_function._func(self_function._arg)
				coroutine.yield(nil)
			end
		end
		)
	end
	
	self._id_table[id] = index
	self._size = index
	
--	self._dump_func("CroutineCreated\n")
--	self:Dump()
	return id
end

--���f
function CoroutineManager:Yield()
	coroutine.yield(false)
end

--�w�莞�ԑҋ@(ms)(���e�X�g)

function CoroutineManager:Wait(ms)
	coroutine.yield(ms)
end

--�R���[�`�����s(�I��������true,���ĂȂ�������false)
function CoroutineManager:processCoroutine(co)
	-- ���̎��s���ԂɂȂ��Ă��Ȃ���΁A�������Ȃ�
	local wakeup_ticks = co._wakeup_ticks
	if (wakeup_ticks == false) or (getTicks() >= wakeup_ticks) then
		co._wakeup_ticks = false

		-- �o�^����Ă���֐��̎��s
		local ret, alive = coroutine.resume(co._co,co._arg)
		if ret == false then
			-- �G���[���b�Z�[�W�̏o��
			-- !!! �t�@�C���A�s�ԍ��Ȃǂ��\��������
			GameSystem.PrintToConsoleAndScreen("�R���[�`���̎��s�ŃG���[���������܂����B:" ..alive .."\n")
			error ("�R���[�`���̎��s�ŃG���[���������܂����B:" ..alive .."\n")
			return true
		end

		if alive == nil then
            
--			self._dump_func("Coroutinr disposed\n")
--			self:Dump()
            
			return true

		elseif type(alive) == 'number' then
			-- ���̋N�����Ԃ�o�^
			co._wakeup_ticks = getTicks() + alive
		end
	end
	return false
end

--�Ǘ�����
function CoroutineManager:Process()
	local terminated_id = {}

	for i = 1, self._size do
		local self_function = self._coroutines[i]
		if self_function._id >= 0 then

			-- �I�������R���[�`���� index ���L�^���Ă����A�Ō�ɍ폜����
			if self:processCoroutine(self_function) == true then
				table.insert(terminated_id, i)
			end
		end
	end

	-- �I�������R���[�`���𖖔��̃R���[�`���Ɠ���ւ�
	for key, value in pairs(terminated_id) do
	    local t_co = self._coroutines[value]
		self._id_table[self._coroutines[self._size]._id] = value
		self._coroutines[value], self._coroutines[self._size] = self._coroutines[self._size], self._coroutines[value]
		self._size = self._size - 1
		self._id_table[t_co._id] = nil
--		self._dump_func("CoroutineEneded:"..value.."\n")
	end
--	return self._size
end

--�w��ID�̃R���[�`�����L����(���e�X�g)
function CoroutineManager:IsActive(id)
	if self._id_table[id] == nil then
		return false
	else
		return true
	end
end

--�w��ID�̃R���[�`�����s(1�ŏI��)
function CoroutineManager:ExcuteCoroutine(id)
	local idx = self._id_table[id]
--	GameSystem.PrintToConsole("Excute coroutine.ID:" ..id ..",idx:" ..idx .."," ..self._coroutines[idx]._id .."\n")
	if idx == nil then
	    GameSystem.PrintToConsole("�w��ID�̃R���[�`���͑��݂��܂���B"..id.."\n")
		return -1
	else
		if self:processCoroutine(self._coroutines[idx])==true then
		    local t_co = self._coroutines[idx]
			self._id_table[self._coroutines[self._size]._id] = idx
			self._coroutines[idx], self._coroutines[self._size] = self._coroutines[self._size], self._coroutines[idx]
			self._size = self._size - 1
			self._id_table[id] = nil
			return 1
		else
			return 0
		end
	end
end

--�w��ID�̃R���[�`��������
function CoroutineManager:InvalidateCoroutine(id)
	local idx = self._id_table[id]
	if idx == nil then
	    GameSystem.PrintToConsole("�w��ID�̃R���[�`���͑��݂��܂���B"..id.."\n")
		return -1
	else
		--�����̃R���[�`���Ɠ���ւ�
		local idx = self._id_table[id]
		local t_co = self._coroutines[idx]
		self._id_table[self._coroutines[self._size]._id] = idx
		self._coroutines[idx], self._coroutines[self._size] = self._coroutines[self._size], self._coroutines[idx]
		self._size = self._size - 1
		self._id_table[id] = nil
	    t_co._co=nil --�R���[�`��������ɂ��邱�Ƃōė��p���ɐV�K�쐬������
		return 0
	end
end

--�T�C�Y�擾
function CoroutineManager:GetCount()
	return self._size
end

--������
function CoroutineManager:Init()
	self._coroutines={}
	self._id_table={}
	self._size=0
	self._id_counter=1
end

function CoroutineManager:SetDumpFunction(df)
    self._dump_func = df
end

--�Ǘ����\��
function CoroutineManager:Dump()
	local dump = self._dump_func
	dump("---CoroutineManager---\n")
	dump("Valid coroutine cnt:"..self._size.."\n")
	dump("Holding coroutine cnt"..#self._coroutines.."\n")
	dump("valid coroutine list\n")
	for key,value in pairs(self._id_table) do
	    dump(" id:"..key)
	    dump(",idx:"..value)
	    local crtn = self._coroutines[value]
	    dump(",status:")
	    if crtn==nil then
	        dump("Invalid\n")
	    else
	        dump("Valid\n")
        end
	end
	dump("----------------------\n")
end