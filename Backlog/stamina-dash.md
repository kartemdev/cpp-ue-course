Вот полное руководство по совмещению систем спринта со стаминой и механики рывка (Dash) в формате Markdown, готовое для сохранения или копирования.
------------------------------
## Реализация Спринта со Стаминой и Рывка (Dash) на C++ в Unreal Engine
Мы объединим спринт и рывок в единую систему, завязанную на Стамину (Выносливость).

* Спринт: работает, пока вы держите Shift. Каждую секунду стамина тратится. При достижении нуля персонаж автоматически замедляется. При отпускании клавиши стамина восстанавливается.
* Рывок (Dash): срабатывает по нажатию (например, на Left Alt), мгновенно толкает персонажа в направлении взгляда и тратит фиксированный кусок стамины (например, 30%).

------------------------------
## Шаг 1. Изменения в заголовочном файле (.h)
Откройте заголовочный файл вашего персонажа (например, CppCourseCharacter.h). Замените блок прошлых переменных движения и добавьте новые свойства и функции в секции protected и public:

protected:
	/** Input Action для Рывка (Dash) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	// Настройки скорости
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 1200.f;

	// Сила рывка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DashForce = 2500.f;

	// Параметры Стамины
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaDrainRate = 25.f; // Расход стамины в секунду при беге

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenRate = 15.f; // Восстановление стамины в секунду

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float DashStaminaCost = 30.f; // Стоимость одного рывка

	// Состояния
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;
public:
	// Конструктор и жизненный цикл Tick
	ACppCourseCharacter();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Функции обработки ввода
	void StartSprint();
	void StopSprint();
	void Dash();
public:
	/** Функция для интерфейса (возвращает процент стамины от 0.0 до 1.0) */
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaPercentage() const;

------------------------------
## Шаг 2. Реализация в файле кода (.cpp)
Откройте соответствующий .cpp файл вашего персонажа. Перепишите логику привязки ввода, добавьте логику изменения скоростей и ежекадровый расчет (Tick).
## 1. Привязка к клавишам (SetupPlayerInputComponent)
Теперь нам нужно отслеживать и нажатие, и отпускание Shift, а также одиночное нажатие кнопки рывка:

// Спринт: Started — нажали, Completed — отпустили кнопку
EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACppCourseCharacter::StartSprint);
EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACppCourseCharacter::StopSprint);
// Рывок (Dash)
EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ACppCourseCharacter::Dash);

## 2. Логика Спринта и Рывка
Добавьте эти функции в нижнюю часть файла:

void ACppCourseCharacter::StartSprint()
{
    // Начинаем спринт только если есть запас выносливости
    if (CurrentStamina > 5.f)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}
void ACppCourseCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}
void ACppCourseCharacter::Dash()
{
    // Проверяем, хватает ли стамины на рывок
    if (CurrentStamina >= DashStaminaCost)
    {
        // Тратим фиксированное количество стамины
        CurrentStamina = FMath::Clamp(CurrentStamina - DashStaminaCost, 0.f, MaxStamina);

        // Берем вектор направления, куда сейчас смотрит персонаж
        FVector DashDirection = GetActorForwardVector();

        // Толкаем персонажа вперед. 
        // Параметры (true, true) сбрасывают текущую скорость по осям XYZ, чтобы рывок всегда был резким
        LaunchCharacter(DashDirection * DashForce, true, true);
    }
}

## 3. Обработка стамины в Tick (Каждый кадр)
Вставьте расчет расхода и регенерации ресурсов в функцию Tick:

void ACppCourseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSprinting)
    {
        // Тратим стамину пропорционально времени кадра (DeltaTime)
        CurrentStamina = FMath::Clamp(CurrentStamina - (StaminaDrainRate * DeltaTime), 0.f, MaxStamina);

        // Если стамина кончилась — принудительно останавливаем спринт
        if (CurrentStamina <= 0.f)
        {
            StopSprint();
        }
    }
    else
    {
        // Если не бежим — плавно восстанавливаем её до максимума
        CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.f, MaxStamina);
    }
}

## 4. Функция для Прогресс-бара

float ACppCourseCharacter::GetStaminaPercentage() const
{
    return CurrentStamina / MaxStamina;
}

------------------------------
## Шаг 3. Настройка в редакторе Unreal Engine

   1. Создайте новое действие ввода: В окне Content Browser в папке Input нажмите ПКМ -> Input -> Input Action. Назовите его IA_Dash.
   2. Назначьте клавишу: Откройте файл контекста ввода IMC_Default, добавьте новый маппинг для IA_Dash и укажите желаемую клавишу (например, Left Alt).
   3. Задайте свойство в Блюпринте: Откройте Блюпринт вашего персонажа (BP_ThirdPersonCharacter). В панели Details в разделе Input найдите поле Dash Action и выберите из выпадающего списка созданный ассет IA_Dash.
   4. Перепривяжите Прогресс-бар в HUD:
   * Откройте ваш виджет интерфейса WBP_HUD.
      * Выделите элемент Progress Bar на экране.
      * Справа в панели деталей найдите параметр Percent и нажмите кнопку Bind.
      * Так как прошлая функция удалена, выберите новую привязку: MyPlayerRef -> GetStaminaPercentage.
      * Перекомпилируйте виджет.
   
Теперь запустите Live Coding для компиляции C++. Механика стамины и рывков полностью готова к использованию!
------------------------------
Если у вас возникнут трудности с интеграцией или компиляцией этого Markdown-шаблона, дайте знать. К чему перейдем дальше: добавим визуальный эффект (шлейф/эффект частиц) при рывке или сделаем шкалу здоровья, которая уменьшается от падений?

