// // #include "ObjectSpawnComponent.h"

// ObjectSpawnComponent::ObjectSpawnComponent(Game* game, Spawner spawnerType, int cooldown, int cooldownVariation, float chance)
//   :
//   GameComponent(game),
//   spawnerType(spawnerType),
//   timer(cooldown),
//   cooldown(cooldown),
//   cooldownVariation(cooldownVariation),
//   chance(chance)
// {
//   switch (spawnerType)
//   {
//   case Spawner::saw:
//     spawner = [game]()
//     {
//       game->SpawnObject(std::make_unique<Saw>(game->GetWorldBounds()));
//     };
//     break;
//   case Spawner::target:
//     spawner = [game]()
//     {
//       game->SpawnObject(std::make_unique<MovingTarget>(game->GetWorldBounds()));
//     };
//     break;
//   case Spawner::time:
//     spawner = [game]()
//     {
//       game->SpawnObject(std::make_unique<TimeBonus>(game->GetWorldBounds()));
//     };
//     break;
  
//   default:
//     break;
//   }
// }

// void ObjectSpawnComponent::Update()
// {
//   timer -= Clock::Delta();

//   while (timer < 0)
//   {
//     std::uniform_int_distribution dist(-cooldownVariation, cooldownVariation);
//     std::uniform_real_distribution<float> c;

//     timer += cooldown + dist(Utility::rng);
//     if (c(Utility::rng) < chance)
//       spawner();
//   }
// }

// void ObjectSpawnComponent::ProcessEvents(Event& event)
// {
//   switch (event.type)
//   {
//   case Event::Type::timerRefill:
//     if (spawnerType == Spawner::time)
//       chance *= 0.5f;
//     break;
  
//   default:
//     break;
//   }
// }
