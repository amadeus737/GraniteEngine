#pragma once

#include "Core.h"
#include <queue>
#include <bitset>

namespace Granite
{
	class GRANITE_API Keyboard
	{
		friend class Window;

	public:
		class Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};

		public:
			// Constructors
			Event();
			Event(Type type, unsigned char code);

			// Getters
			bool IsPress() const;
			bool IsRelease() const;
			bool IsValid() const;
			unsigned char GetKeyCode() const;

		private:
			Type _type;
			unsigned char _keyCode;
		};

	public:
		bool KeyIsPressed(unsigned char keycode) const;
		Event ReadKey();
		bool KeyIsEmpty() const;
		char ReadChar();
		bool CharIsEmpty() const;
		void FlushKey();
		void FlushChar();
		void Flush();
		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatIsEnabled() const;

	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);
		void ClearState();
		template<typename T>
		void TrimBuffer(std::queue<T>& buffer);

	public:
		static constexpr unsigned int _nKeys = 256u;

	private:
		static constexpr unsigned int _bufferSize = 4u;
		bool _autorepeatEnabled = false;
		std::queue<Event> _keybuffer;
		std::queue<char> _charbuffer;
	};
}