import re


def is_legit_citizen_id(id_card):

    areas = [11, 12, 13, 14, 15, 21, 22, 23, 31, 32, 33, 34, 35, 36, 37, 41, 42, 43, 44, 45, 46, 50, 51, 52, 53, 54, 61,
             62, 63, 64, 65, 71, 81, 82, 91]
    id_card = str(id_card)

    id_card = id_card.strip()  # 删除前后空格

    id_card_list = list(id_card)

    if len(id_card) == 18:

        # 出生日期的合法性检查
        if int(id_card[0:2]) not in areas:
            return False

        if int(id_card[6:10]) % 4 == 0 or (int(id_card[6:10]) % 100 == 0 and int(id_card[6:10]) % 4 == 0):

            e_reg = re.compile(

                '[1-9][0-9]{5}(19[0-9]{2}|20[0-9]{2})((01|03|05|07|08|10|12)(0[1-9]|[1-2][0-9]|3[0-1])|(04|06|09|11)'

                '(0[1-9]|[1-2][0-9]|30)|02(0[1-9]|[1-2][0-9]))[0-9]{3}[0-9Xx]$')  # //闰年出生日期的合法性正则表达式

        else:

            e_reg = re.compile(

                '[1-9][0-9]{5}(19[0-9]{2}|20[0-9]{2})((01|03|05|07|08|10|12)(0[1-9]|[1-2][0-9]|3[0-1])|(04|06|09|11)'

                '(0[1-9]|[1-2][0-9]|30)|02(0[1-9]|1[0-9]|2[0-8]))[0-9]{3}[0-9Xx]$')  # //平年出生日期的合法性正则表达式

        # //测试出生日期的合法性

        if re.match(e_reg, id_card):

            # //计算校验位

            s = (int(id_card_list[0]) + int(id_card_list[10])) * 7 + (int(id_card_list[1]) +

                                                                      int(id_card_list[11])) * 9 + (

                        int(id_card_list[2]) + int(id_card_list[12])) * 10 + (

                        int(id_card_list[3]) + int(id_card_list[13])) * 5 + (

                        int(id_card_list[4]) + int(id_card_list[14])) * 8 + (

                        int(id_card_list[5]) + int(id_card_list[15])) * 4 + (

                        int(id_card_list[6]) + int(id_card_list[16])) * 2 + int(id_card_list[7]) * 1 + int(

                id_card_list[8]) * 6 + int(id_card_list[9]) * 3

            y = s % 11

            m = "F"

            jym = "10X98765432"

            m = jym[y]  # 判断校验位

            print(s)

            print(y)

            print(m)

            if m == id_card_list[17]:  # 检测ID的校验位

                return True

            else:

                return False

        else:

            return False

    else:

        return False




