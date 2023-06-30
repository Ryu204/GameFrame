#ifndef GAMEFRAME_AUDIO_CONTEXT_BASED_WRAPPER_HPP
#define GAMEFRAME_AUDIO_CONTEXT_BASED_WRAPPER_HPP

/*
    Base class for resources that require one context and device is available before creation
*/

namespace HJUIK
{
    namespace Audio
	{
        // Derived class require that at least one device is available at its creation
        class ContextBasedWrapper
        {
        public:
            ContextBasedWrapper();
            virtual ~ContextBasedWrapper();

            ContextBasedWrapper(ContextBasedWrapper&&) = delete;
            ContextBasedWrapper(const ContextBasedWrapper&) = delete;
            auto operator=(ContextBasedWrapper&&) -> ContextBasedWrapper& = delete;
            auto operator=(const ContextBasedWrapper&) -> ContextBasedWrapper& = delete;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
