#pragma once


namespace mcu {
	struct nvic_registers;

	class sys_nvic {
		friend class system;
	public:

	private:
		nvic_registers* nvic;
		nvic_registers* nvic_stir;
		sys_nvic() = default;
		void configure();
		static void sv_call();
		static void pend_sv();
	};
}