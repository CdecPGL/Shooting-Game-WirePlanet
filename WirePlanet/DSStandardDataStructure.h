#pragma once

#include"DataScript.h"

//�f�[�^�X�N���v�g�W���f�[�^�\���֘A
namespace DSsds{
	/**
	* @brief �f�[�^�����݂��Ȃ��ꍇ�ɕԂ���镶����
	*/
	const std::string not_exist_string("NULL");
	/**
	* @brief �f�[�^�[ID�ɑΉ������f�[�^�u���b�N�����擾
	* @param �f�[�^ID
	* @return �f�[�^�u���b�N���B���݂��Ȃ��ꍇ��not_exist_string
	*/
	const std::string GetDataBlockNameByDataID(const DataScript&, const std::string&);
	/**
	* @brief �f�[�^�[ID���w�肵�ăf�[�^�u���b�N���擾
	* @param �f�[�^ID
	* @return �f�[�^�u���b�N�|�C���^�B���݂��Ȃ��ꍇ��nullptr
	*/
	std::shared_ptr<const DataBlock> GetDataBlockByDataID(const DataScript&, const std::string&);
	/**
	* @brief �w��f�����f�[�^�[ID���L�����m�F�B
	* @param �f�[�^ID
	* @return �L���ȏꍇ��true,�����ȏꍇ��false�BID�͓o�^����Ă��邪�A�Ή�����f�[�^�u���b�N�����݂��Ȃ��ꍇ��false�ƂȂ�B
	*/
	bool IsValidDataID(const DataScript&, const std::string&);
	/**
	* @brief DS��DSdsd���m�F
	* @param DS
	* @return true or false
	*/
	bool IsDSSDS(const DataScript&);

}