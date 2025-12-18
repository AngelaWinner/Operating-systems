package com.example.todoapi.repository;

import com.example.todoapi.model.Task;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

@Repository
public class InMemoryTaskRepository implements TaskRepository {

    private final Map<Integer, Task> tasks = new ConcurrentHashMap<>();

    private final AtomicInteger idCounter = new AtomicInteger(1);

    @Override
    public List<Task> findAll() {
        return new ArrayList<>(tasks.values());
    }

    @Override
    public Task findById(int id) {
        return tasks.get(id);
    }

    @Override
    public Task save(Task task) {
        int newId = idCounter.getAndIncrement();

        task.setId(newId);

        if (task.getStatus() == null || task.getStatus().getValue().isEmpty()) {
            task.setStatus("todo");
        }

        tasks.put(newId, task);

        System.out.println("Создана новая задача: " + task);
        return task;
    }

    @Override
    public Task update(int id, Task updatedTask) {
        Task existingTask = tasks.get(id);
        if (existingTask == null) {
            System.out.println("Задача с ID=" + id + " не найдена для обновления");
            return null;
        }

        updatedTask.setId(id);

        if (updatedTask.getStatus() == null || updatedTask.getStatus().getValue().isEmpty()) {
            updatedTask.setStatus(existingTask.getStatus());
        }

        if (updatedTask.getTitle() == null || updatedTask.getTitle().isEmpty()) {
            updatedTask.setTitle(existingTask.getTitle());
        }

        if (updatedTask.getDescription() == null) {
            updatedTask.setDescription(existingTask.getDescription());
        }

        tasks.put(id, updatedTask);

        System.out.println("Обновлена задача с ID=" + id + ": " + updatedTask);
        return updatedTask;
    }

    @Override
    public boolean delete(int id) {
        Task removedTask = tasks.remove(id);
        if (removedTask != null) {
            System.out.println("Удалена задача с ID=" + id + ": " + removedTask);
            return true;
        } else {
            System.out.println("Задача с ID=" + id + " не найдена для удаления");
            return false;
        }
    }

    public void printAllTasks() {
        System.out.println("=== Текущее состояние хранилища ===");
        if (tasks.isEmpty()) {
            System.out.println("Хранилище пусто");
        } else {
            tasks.forEach((id, task) ->
                    System.out.println("ID " + id + ": " + task)
            );
        }
        System.out.println("===================================");
    }

    public void clear() {
        tasks.clear();
        idCounter.set(1);
        System.out.println("Хранилище очищено");
    }
}