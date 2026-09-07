"""Exact prompt and retry text used by all commercial LLM baselines."""

SYSTEM_INSTRUCTION = (
    "You are a security code auditor. Given a C function, decide whether it "
    "contains a software vulnerability. Answer with exactly one word: "
    '"VULNERABLE" or "SAFE". Do not explain.'
)

RETRY_INSTRUCTION = (
    "Your previous response could not be parsed because it was not exactly "
    "VULNERABLE or SAFE. For the same function, return exactly one word: "
    "VULNERABLE or SAFE. Do not explain."
)


def label_text(label):
    return "VULNERABLE" if int(label) == 1 else "SAFE"


def build_user_prompt(demonstrations, query_code):
    """Render five examples followed by the target function."""
    if len(demonstrations) != 5:
        raise ValueError("the protocol requires exactly five demonstrations")
    blocks = ["Here are five labeled examples."]
    for index, item in enumerate(demonstrations, start=1):
        blocks.append(
            "Example %d:\n%s\nLabel: %s" %
            (index, item["code"], label_text(item["label"]))
        )
    blocks.append("Now classify the following function.\n%s\nLabel:" % query_code)
    return "\n\n".join(blocks)

