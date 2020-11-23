#pragma once


#ifndef WFL_NON_INSTANTIATABLE


// �C���X�^���X���s��
#define WFL_NON_INSTANTIATABLE(class_name) \
class_name() = delete; \
~class_name() = delete; \


#endif // WFL_NON_INSTANTIATABLE

