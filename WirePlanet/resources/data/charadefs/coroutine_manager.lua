--http://hyakuren-soft.sakura.ne.jp/games_html/lua_coroutine_page.htmlを参照
--2014/11/4 Version 2.0 不具合修正
--コルーチンの終了時は何も返してはならない
module( "coroutinemanager", package.seeall )

CoroutineManager = {}

--コルーチンマネージャ
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

--コルーチン作成
function CoroutineManager:CreateCoroutine(func,arg)
--    self:Dump()
	local index = self._size + 1
	local id 
	local self_function --作成先
	
	if self._size >= #self._coroutines then
	-- コルーチンの新規作成
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
		-- コルーチンの再利用
		self_function = self._coroutines[index]
		self_function._func = func
		self_function._arg = arg
		self_function._wakeup_ticks = false
		id = self_function._id
	end
	
	--コルーチンが未作成だったら作成する
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

--中断
function CoroutineManager:Yield()
	coroutine.yield(false)
end

--指定時間待機(ms)(未テスト)

function CoroutineManager:Wait(ms)
	coroutine.yield(ms)
end

--コルーチン実行(終了したらtrue,してなかったらfalse)
function CoroutineManager:processCoroutine(co)
	-- 次の実行時間になっていなければ、処理しない
	local wakeup_ticks = co._wakeup_ticks
	if (wakeup_ticks == false) or (getTicks() >= wakeup_ticks) then
		co._wakeup_ticks = false

		-- 登録されている関数の実行
		local ret, alive = coroutine.resume(co._co,co._arg)
		if ret == false then
			-- エラーメッセージの出力
			-- !!! ファイル、行番号なども表示させる
			GameSystem.PrintToConsoleAndScreen("コルーチンの実行でエラーが発生しました。:" ..alive .."\n")
			error ("コルーチンの実行でエラーが発生しました。:" ..alive .."\n")
			return true
		end

		if alive == nil then
            
--			self._dump_func("Coroutinr disposed\n")
--			self:Dump()
            
			return true

		elseif type(alive) == 'number' then
			-- 次の起床時間を登録
			co._wakeup_ticks = getTicks() + alive
		end
	end
	return false
end

--管理処理
function CoroutineManager:Process()
	local terminated_id = {}

	for i = 1, self._size do
		local self_function = self._coroutines[i]
		if self_function._id >= 0 then

			-- 終了したコルーチンの index を記録しておき、最後に削除する
			if self:processCoroutine(self_function) == true then
				table.insert(terminated_id, i)
			end
		end
	end

	-- 終了したコルーチンを末尾のコルーチンと入れ替え
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

--指定IDのコルーチンが有効か(未テスト)
function CoroutineManager:IsActive(id)
	if self._id_table[id] == nil then
		return false
	else
		return true
	end
end

--指定IDのコルーチン実行(1で終了)
function CoroutineManager:ExcuteCoroutine(id)
	local idx = self._id_table[id]
--	GameSystem.PrintToConsole("Excute coroutine.ID:" ..id ..",idx:" ..idx .."," ..self._coroutines[idx]._id .."\n")
	if idx == nil then
	    GameSystem.PrintToConsole("指定IDのコルーチンは存在しません。"..id.."\n")
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

--指定IDのコルーチン無効化
function CoroutineManager:InvalidateCoroutine(id)
	local idx = self._id_table[id]
	if idx == nil then
	    GameSystem.PrintToConsole("指定IDのコルーチンは存在しません。"..id.."\n")
		return -1
	else
		--末尾のコルーチンと入れ替え
		local idx = self._id_table[id]
		local t_co = self._coroutines[idx]
		self._id_table[self._coroutines[self._size]._id] = idx
		self._coroutines[idx], self._coroutines[self._size] = self._coroutines[self._size], self._coroutines[idx]
		self._size = self._size - 1
		self._id_table[id] = nil
	    t_co._co=nil --コルーチンを似るにすることで再利用時に新規作成させる
		return 0
	end
end

--サイズ取得
function CoroutineManager:GetCount()
	return self._size
end

--初期化
function CoroutineManager:Init()
	self._coroutines={}
	self._id_table={}
	self._size=0
	self._id_counter=1
end

function CoroutineManager:SetDumpFunction(df)
    self._dump_func = df
end

--管理情報表示
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