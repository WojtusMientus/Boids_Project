#pragma once


#define ENSURE_ALWAYS_RETURN(Expression)\
	if (!ensureAlways(Expression))\
		{\
			return;\
		}

#define ENSURE_ALWAYS_RETURN_VALUE(Expression, ReturnValue)\
	if (!ensureAlways(Expression))\
		{\
			return ReturnValue;\
		}


#define ENSURE_ALWAYS_CONTINUE(Expression)\
	if (!ensureAlways(Expression))\
		{\
			continue;\
		}

#define ENSURE_ALWAYS_MESSAGE_RETURN(Expression, Message)\
	if (!ensureAlwaysMsgf(Expression, TEXT(Message)))\
		{\
			return;\
		}

#define ENSURE_ALWAYS_MESSAGE_RETURN_VALUE(Expression, Message, ReturnValue)\
	if (!ensureAlwaysMsgf(Expression, TEXT(Message)))\
		{\
			return ReturnValue;\
		}


#define ENSURE_ALWAYS_MESSAGE_CONTINUE(Expression, Message)\
	if (!ensureAlwaysMsgf(Expression, TEXT(Message)))\
		{\
			continue;\
		}

