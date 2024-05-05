import pyperclip3 as pc
import os

default_prompt = (
    "**Use this layout (keep the comments of the layout, remove comments of the given prototypes), provide only the answer:**\n"
    "```c\n"
    "/* Pin toggle function (without static parameter checking) */\n"
    "uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin);\n"
    "/* Pin toggle function (with static parameter checking) */\n"
    "uhal_status_t GPIO_TOGGLE_PIN_OUTPUT(const gpio_pin_t pin);\n"

    "/* Function to set the pin mode (without static parameter checking) */\n"
    "uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);\n"
    "/* Function to set the pin mode (with static parameter checking) */\n"
    "uhal_status_t GPIO_SET_PIN_MODE(const gpio_pin_t pin, gpio_mode_t pin_mode);\n"
    "```\n"
    "**Using these prototypes, these functions also have a matching uppercase version with compile-time checking, as shown above give both the uppercase and lowercase prototypes of these functions!**\n"
    "```c\n")

function_prototypes = []


# Function to replace content in the file based on the replacements dictionary
def replace_content(template_file_path, answer_file_path, replacements):
    # Read the file
    with open(template_file_path, 'r') as file:
        content = file.read()

    # Replace placeholders with actual values
    for key, value in replacements.items():
        content = content.replace(key, value)

    # Write the modified content back to the file
    with open(answer_file_path, 'w') as file:
        file.write(content)


def append_content(answer_file_path, content):
    with open(answer_file_path, 'a') as file:
        file.write(content)

def replace_and_print(template_file_path, replacements, print_message):
    # Read the file
    with open(template_file_path, 'r') as file:
        content = file.read()
    # Replace placeholders with actual values
    for key, value in replacements.items():
        content = content.replace(key, value)

    print(print_message + '\n')
    print(content)
    pc.copy(content)

def read_multiline_resp():
    contents = []
    while True:
        line = input()
        if (line == ';'):
            break
        contents.append(line + '\n')
    return ''.join(contents)

def main():
    module_name = input("What is the module name: ")
    module_name_unabbreviated = input("What is the unabbreviated name of the module: ")
    number_of_functions_to_generate = input("How many API functions does this module have: ")
    for i in range(0, int(number_of_functions_to_generate)):
        print("Enter function prototype " + str(i) + ":")
        print("Enter/Paste your content. Press ENTER -> ';' Followed again by ENTER")
        new_function_prototype = read_multiline_resp()
        function_prototypes.append(new_function_prototype + '\n')
    prompt_function_description = default_prompt + ''.join(function_prototypes) + "```"
    os.system('clear')
    print("Paste this response in to chatgpt: \n\n\n")
    print(prompt_function_description)
    pc.copy(prompt_function_description)
    print("\n\n\n Paste the response of chatgpt here in the terminal:")
    print("Enter/Paste your content. Press ENTER -> ';' Followed again by ENTER")
    prompt_response = read_multiline_resp()

    content_to_be_replaced = {"[MODULE_NAME]": module_name,
                              "[MODULE_NAME_UNABBREVIATED]": module_name_unabbreviated,
                              "[MODULE_API_FUNCTIONS]": prompt_response,
                              }
    replace_content("template.md", "answer.md", content_to_be_replaced)

    content_to_be_replaced.clear()
    for i in function_prototypes:
        content_to_be_replaced = {"[FUNCTION_PROTOTYPE]": i,
                                 }
        replace_and_print("template_function_section.txt", content_to_be_replaced, "Paste this into chatgpt \n\n")
        print("\n\n\n Paste the response of chatgpt here in the terminal:")
        print("Enter/Paste your content. Press ENTER -> ';' Followed again by ENTER")
        prompt_response = read_multiline_resp()
        append_content("answer.md", prompt_response)

if __name__ == "__main__":
    main()
